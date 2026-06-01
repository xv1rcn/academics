% !TeX program = xelatex
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{academics}[2026/06/01 Reusable academics lab report class]

% ---- class loading ----

\PassOptionsToClass{12pt, a4paper}{ctexart}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{ctexart}}
\ProcessOptions\relax
\LoadClass{ctexart}

% ---- page & font ----

\RequirePackage[margin=1in]{geometry}

\setmonofont{JetBrainsMonoNerdFont}[
    Path           = fonts/,
    Extension      = .ttf,
    UprightFont    = *-Regular,
    BoldFont       = *-Bold,
    ItalicFont     = *-Italic,
    BoldItalicFont = *-BoldItalic
]

% ---- section & paragraph ----

\RequirePackage{titlesec}
\RequirePackage{zhnumber}
\titleformat*{\section}{\Large\bfseries\raggedright}
\renewcommand{\thesection}{\zhnum{section}}
\renewcommand{\thesubsection}{\arabic{subsection}}

\setcounter{secnumdepth}{4}
\renewcommand{\paragraph}[1]{%
    \par{\bfseries#1}\quad
    \noindent
}

% ---- lists & spacing ----

\RequirePackage{enumitem}
\setlist[enumerate]{itemsep=2pt, parsep=0pt, partopsep=0pt, topsep=2pt}
\setlist[itemize]{itemsep=2pt, parsep=0pt, partopsep=0pt, topsep=2pt}
\linespread{1.2}
\setlength{\parindent}{2em}

% ---- math & symbols ----

\RequirePackage{dingbat}    % 必须在 amsmath / amssymb 之前，避免 \checkmark 冲突
\RequirePackage{amsmath}
\RequirePackage{amssymb}
\RequirePackage{indentfirst}

% ---- code ----

\RequirePackage{minted}
\setminted{
    breakanywhere, breaklines, breakautoindent=false,
    breaksymbolindentleft=0pt, breaksymbolindentright=0pt,
    breaksymbolleft=\raisebox{0.8ex}{\small\reflectbox{\carriagereturn}},
    breaksymbolright=\small\carriagereturn,
    breaksymbolsepleft=0pt, breaksymbolsepright=0pt,
    escapeinside=||, fontsize=\footnotesize,
    frame=single, framesep=1em, mathescape, style=bw, tabsize=4
}

\RequirePackage{xparse}
\NewDocumentCommand{\mil}{ O{text} m }{%
    \mintinline{#1}{#2}%
}

% ---- boxes ----

\RequirePackage[breakable]{tcolorbox}

\makeatletter

\tcbset{
    arc=0pt, boxrule=.8pt, breakable,
    before upper={\setlength{\parindent}{2em}\par},
    after upper={\par},
    width=\dimexpr\linewidth+\@totalleftmargin\relax,
    enlarge left by=-\@totalleftmargin
}

\makeatother

% ---- graphics & units ----

\RequirePackage{graphicx}
\RequirePackage{siunitx}
\RequirePackage{tikz}

% ---- cover page ----

\makeatletter

\def\academics@coverlabel{实验报告}
\def\academics@coursecn{}
\def\academics@courseen{}
\def\academics@reporttitle{}
\def\academics@reportdate{}
\def\academics@authorname{}
\def\academics@authorid{}

\newcommand{\academicsCoverLabel}[1]{\def\academics@coverlabel{#1}}
\newcommand{\academicsCourse}[2]{%
    \def\academics@coursecn{#1}%
    \def\academics@courseen{#2}%
}
\newcommand{\academicsReport}[2]{%
    \def\academics@reporttitle{#1}%
    \def\academics@reportdate{#2}%
}
\newcommand{\academicsArthur}[2]{%
    \def\academics@authorname{#1}%
    \def\academics@authorid{#2}%
}

\newcommand{\makeacademicscover}{%
    \pagestyle{plain}
    \thispagestyle{empty}
    \noindent
    \begin{tabular*}{\textwidth}{l @{\extracolsep{\fill}} r @{\extracolsep{6pt}} l}
        \LARGE{\textbf{\academics@coverlabel}} & \academics@coursecn & \textit{\academics@courseen} \\
    \end{tabular*}\\\\
    \begin{tabular*}{\textwidth}{@{}p{0.5\textwidth}@{}p{0.5\textwidth}@{}}
        \textbf{标题: } \textbf{\academics@reporttitle} & \textbf{日期: } \academics@reportdate \\
        \textbf{姓名: } \academics@authorname & \textbf{学号: } \academics@authorid \\
    \end{tabular*}\\
    \rule[2ex]{\textwidth}{2pt}
}

\makeatother
