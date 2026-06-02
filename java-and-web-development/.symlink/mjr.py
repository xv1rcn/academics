#!/usr/bin/env python3
"""
mjr.py : Minimal Java Runner by xv1rcn
Last updated on Jun. 2, 2026.
Run `mjr.py --help` for usages.
"""

from __future__ import annotations

import argparse
import os
import pty
import re
import select
import shutil
import signal
import subprocess
import sys
import termios
import tty

from pathlib import Path
from typing import Optional, List, Tuple, Union


class Output:

    @staticmethod
    def info(msg: str) -> None:
        print(f"[*] {msg}", file=sys.stderr)

    @staticmethod
    def ok(msg: str) -> None:
        print(f"[+] {msg}", file=sys.stderr)

    @staticmethod
    def warn(msg: str) -> None:
        print(f"[!] {msg}", file=sys.stderr)

    @staticmethod
    def error(msg: str) -> None:
        print(f"[x] {msg}", file=sys.stderr)

    @staticmethod
    def die(msg: str, code: int = 1) -> None:
        Output.error(msg)
        raise SystemExit(code)

    @staticmethod
    def blank() -> None:
        print(file=sys.stderr)

    @staticmethod
    def separator() -> None:
        print("---", file=sys.stderr)

    @classmethod
    def header(cls, script_name: str) -> None:
        cls.info(f"{script_name}: Minimal Java Runner - by xv1rcn")

    @classmethod
    def project_info(cls: Output, root: Path, javac_path: str, java_path: str) -> None:
        cls.ok(f"Project root @ -> {root}")
        cls.ok(f"pwd -> {Path.cwd()}")
        cls.ok(f"which(javac) -> {javac_path}")
        cls.ok(f"which(java) -> {java_path}")

    @staticmethod
    def scan_file(rel: str, class_name: str) -> None:
        Output.ok(f"{rel} -> Class: {class_name}")

    @staticmethod
    def compile_file(rel_src: str, rel_class: str) -> None:
        Output.ok(f"{rel_src} -> {rel_class}")

    @staticmethod
    def run_start(main_class: str) -> None:
        Output.info(f"Running class {main_class}...")
        Output.separator()

    @classmethod
    def run_end(cls, exit_code: int, out_path: Optional[Path] = None) -> None:
        cls.separator()
        cls.ok(f"Program exited with code {exit_code}")
        if out_path:
            cls.ok(f"stdin / stdout saved to {out_path}")

    @staticmethod
    def cleanup(rel_build: str, kept: bool = False) -> None:
        if kept:
            Output.warn(f"Keeping {rel_build}")
        else:
            Output.ok(f"{rel_build} removed")


class Utils:
    @staticmethod
    def rel_path(path: Path, base: Path) -> str:
        try:
            return "@/" + str(path.relative_to(base))
        except ValueError:
            return str(path)

    @staticmethod
    def resolve_exec(exec_name: str) -> str:
        p = Path(exec_name).expanduser()
        if p.exists():
            return str(p.resolve())
        resolved = shutil.which(exec_name)
        return resolved if resolved else exec_name


class JavaProject:

    def __init__(self, root: Union[str, Path]) -> None:
        self.root = Path(root).expanduser().resolve()
        if not self.root.exists():
            Output.die(f"Directory does not exist: {self.root}")
        if not self.root.is_dir():
            Output.die("Path must be a directory")

    def find_java_files(self) -> List[Path]:
        return [f for f in self.root.rglob("*.java") if "build" not in f.parts]

    @staticmethod
    def package_name(text: str) -> Optional[str]:
        PACKAGE_RE = re.compile(r"^\s*package\s+([a-zA-Z_][\w.]*?)\s*;\s*$")
        for raw_line in text.splitlines():
            line = raw_line.strip()
            if not line or line.startswith("//") or line.startswith("/*"):
                continue
            m = PACKAGE_RE.match(line)
            return m.group(1) if m else None
        return None

    @staticmethod
    def file_to_main_class(file: Path) -> str:
        try:
            text = file.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            return file.stem
        pkg = JavaProject.package_name(text)
        return f"{pkg}.{file.stem}" if pkg else file.stem

    @staticmethod
    def detect_main_files(files: List[Path]) -> List[Path]:
        needle = "public static void main(String[] args)"
        mains: List[Path] = []
        for f in files:
            try:
                if needle in f.read_text(encoding="utf-8", errors="ignore"):
                    mains.append(f)
            except Exception:
                continue
        return mains

    def choose_main_class(self, files: List[Path], specified: Optional[str]) -> str:
        if specified:
            return specified

        mains = self.detect_main_files(files)
        if not mains:
            Output.die("No main method found, please specify with --main")
        if len(mains) == 1:
            return self.file_to_main_class(mains[0])

        candidates = sorted(
            [(self.file_to_main_class(f), f) for f in mains],
            key=lambda x: x[0],
        )
        if sys.stdin.isatty():
            return self._pick_interactive(candidates)

        Output.warn("Multiple main classes detected:")
        for cls, _ in candidates:
            Output.warn(f"  - {cls}")
        Output.die("Please specify with --main")

    def _pick_interactive(self, candidates: List[Tuple[str, Path]]) -> str:
        Output.warn("Multiple main classes detected:")
        for i, (cls, file) in enumerate(candidates, start=1):
            Output.warn(f"  {i:>2}. {cls}  ({Utils.rel_path(file, self.root)})")
        while True:
            raw = input("Please select the main class number: ").strip() or "1"
            try:
                idx = int(raw)
            except ValueError:
                Output.warn("Please enter a number")
                continue
            if 1 <= idx <= len(candidates):
                return candidates[idx - 1][0]
            Output.warn(f"Number out of range (1..{len(candidates)})")


class JavaCompiler:
    def __init__(self, javac_cmd: str) -> None:
        self.javac = Utils.resolve_exec(javac_cmd)

    def compile(
        self, source_files: List[Path], build_dir: Path, sourcepath: Path, classpath: str = ""
    ) -> Optional[str]:
        cmd = [self.javac, "-encoding", "UTF-8"]
        if classpath:
            cmd += ["-cp", classpath]
        cmd += ["-d", str(build_dir), "-sourcepath", str(sourcepath)]
        cmd += [str(f) for f in source_files]

        proc = subprocess.run(
            cmd, check=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT
        )
        if proc.returncode != 0:
            if proc.stdout:
                sys.stdout.buffer.write(proc.stdout)
            Output.die("javac compilation failed")
        return proc.stdout.decode("utf-8", errors="ignore") if proc.stdout else None


class JavaRunner:
    def __init__(self, java_cmd: str) -> None:
        self.java = Utils.resolve_exec(java_cmd)

    def run(
        self, main_class: str, classpath: Path, out_path: Optional[Path] = None, extra_cp: str = ""
    ) -> int:
        cp = str(classpath)
        if extra_cp:
            cp = cp + ":" + extra_cp
        command = [self.java, "-cp", cp, main_class]
        return self._pty_run(command, out_path)

    @staticmethod
    def _pty_run(command: List[str], out_path: Optional[Path]) -> int:
        out_fp = None
        if out_path:
            out_path.parent.mkdir(parents=True, exist_ok=True)
            out_fp = out_path.open("wb")

        master_fd, slave_fd = pty.openpty()
        proc = subprocess.Popen(
            command,
            stdin=slave_fd,
            stdout=slave_fd,
            stderr=slave_fd,
            close_fds=True,
            preexec_fn=os.setsid,
        )
        os.close(slave_fd)

        stdin_fd = sys.stdin.fileno()
        stdout_fd = sys.stdout.fileno()

        old_tty = None
        if sys.stdin.isatty():
            old_tty = termios.tcgetattr(stdin_fd)
            tty.setraw(stdin_fd)

        stdin_eof = False
        try:
            while proc.poll() is None:
                rlist = [master_fd]
                if sys.stdin.isatty() and not stdin_eof:
                    rlist.append(stdin_fd)

                try:
                    ready, _, _ = select.select(rlist, [], [], 0.1)
                except (select.error, ValueError):
                    break

                if master_fd in ready:
                    try:
                        data = os.read(master_fd, 4096)
                    except OSError:
                        data = b""
                    if not data:
                        break
                    os.write(stdout_fd, data)
                    if out_fp:
                        out_fp.write(data)
                        out_fp.flush()

                if sys.stdin.isatty() and stdin_fd in ready:
                    try:
                        data = os.read(stdin_fd, 4096)
                    except OSError:
                        data = b""
                    if not data:
                        stdin_eof = True
                        break
                    os.write(master_fd, data)
                    if out_fp:
                        out_fp.write(data)
                        out_fp.flush()
        except KeyboardInterrupt:
            try:
                proc.send_signal(signal.SIGINT)
            except ProcessLookupError:
                pass
            JavaRunner._drain_pty(master_fd, stdout_fd, out_fp)
        finally:
            if old_tty is not None:
                termios.tcsetattr(stdin_fd, termios.TCSADRAIN, old_tty)

        if stdin_eof:
            try:
                os.close(master_fd)
            except OSError:
                pass

        if not stdin_eof:
            JavaRunner._drain_pty(master_fd, stdout_fd, out_fp)

        try:
            proc.wait()
        except KeyboardInterrupt:
            proc.send_signal(signal.SIGKILL)
            proc.wait()

        try:
            os.close(master_fd)
        except OSError:
            pass

        if out_fp:
            out_fp.close()

        return proc.returncode

    @staticmethod
    def _drain_pty(master_fd: int, stdout_fd: int, out_fp) -> None:
        while True:
            try:
                r, _, _ = select.select([master_fd], [], [], 0.2)
            except select.error:
                break
            if not r:
                break
            try:
                data = os.read(master_fd, 4096)
            except OSError:
                break
            if not data:
                break
            os.write(stdout_fd, data)
            if out_fp:
                out_fp.write(data)
                out_fp.flush()


class Application:
    def run(self, argv: Optional[List[str]] = None) -> int:
        args = self._parse_args(argv)
        script_name = Path(sys.argv[0]).name

        # Environment
        Output.header(script_name)
        project_root = Path(args.path).expanduser().resolve()
        javac_path = Utils.resolve_exec(args.javac)
        java_path = Utils.resolve_exec(args.java)
        Output.project_info(project_root, javac_path, java_path)
        Output.blank()

        # Scan project
        project = JavaProject(args.path)
        java_files = project.find_java_files()

        Output.info("Scanning classes...")
        if not java_files:
            Output.die("未发现任何 .java 文件")
        for f in java_files:
            Output.scan_file(Utils.rel_path(f, project.root), f.stem)
        Output.blank()

        # Prepare build directory
        build_dir = project.root / "build"
        if build_dir.exists():
            shutil.rmtree(build_dir)
        build_dir.mkdir(parents=True, exist_ok=True)

        # Determine main class
        main_class = project.choose_main_class(java_files, args.main_class)

        # Compile
        compiler = JavaCompiler(javac_path)
        Output.info("Compiling classes...")
        for f in java_files:
            class_file = build_dir / (f.stem + ".class")
            Output.compile_file(
                Utils.rel_path(f, project.root),
                Utils.rel_path(class_file, project.root),
            )
        compile_output = compiler.compile(java_files, build_dir, project.root, args.cp)
        if compile_output:
            sys.stderr.write(compile_output)
        Output.blank()

        # Run
        runner = JavaRunner(java_path)
        out_file = self._resolve_out_path(args.out_path)
        Output.run_start(main_class)
        exit_code = runner.run(main_class, build_dir, out_file, args.cp)
        Output.run_end(exit_code, out_file)
        Output.blank()

        # Cleanup
        if args.build:
            Output.cleanup(Utils.rel_path(build_dir, project.root), kept=True)
        else:
            shutil.rmtree(build_dir, ignore_errors=True)
            Output.cleanup(Utils.rel_path(build_dir, project.root))
        Output.blank()

        Output.info("Exiting...")
        return exit_code

    @staticmethod
    def _parse_args(argv: Optional[List[str]]) -> argparse.Namespace:
        parser = argparse.ArgumentParser(
            prog="mjr.py",
            description="""
            Minimal Java Runner,
            a single-file Java compile & run tool,
            implemented by xv1rcn.
            """,
        )
        parser.add_argument(
            "path",
            nargs="?",
            default=".",
            help="Project root directory (default: current directory)",
        )
        parser.add_argument(
            "-o", "--out", dest="out_path", help="Save runtime stdin/stdout to file"
        )
        parser.add_argument(
            "-m",
            "--main",
            dest="main_class",
            help="Main class (e.g., com.example.Main)",
        )
        parser.add_argument(
            "-b", "--build", action="store_true", help="Keep the build directory"
        )
        parser.add_argument("--javac", default="javac", help="javac command or path")
        parser.add_argument("--java", default="java", help="java command or path")
        parser.add_argument("--cp", default="", help="Extra classpath JARs (colon-separated)")
        return parser.parse_args(argv)

    @staticmethod
    def _resolve_out_path(raw: Optional[str]) -> Optional[Path]:
        if not raw:
            return None
        p = Path(raw).expanduser()
        return (p if p.is_absolute() else (Path.cwd() / p)).resolve()


if __name__ == "__main__":
    app = Application()
    raise SystemExit(app.run())
