% !TeX program = xelatex
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{academics}[2026/04/07 Reusable academics lab report class]

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{ctexart}}
\ProcessOptions\relax
\LoadClass{ctexart}

\RequirePackage[margin=2cm]{geometry}
\RequirePackage{libertine}

\RequirePackage{titlesec}
\RequirePackage{zhnumber}
\titleformat*{\section}{\Large\bfseries\raggedright}
\renewcommand{\thesection}{\zhnum{section}}
\renewcommand{\thesubsection}{\arabic{subsection}}

\setcounter{secnumdepth}{4}
\renewcommand{\theparagraph}{\thesubsubsection.\arabic{paragraph}}
\renewcommand{\paragraph}[1]{%
    \refstepcounter{paragraph}%
    {\bfseries\theparagraph\quad#1}\par
    \vspace{2pt}%
    \noindent
}

\RequirePackage{enumitem}	
\setlist[enumerate]{itemsep=2pt, parsep=0pt, partopsep=0pt, topsep=2pt}
\setlist[itemize]{itemsep=2pt, parsep=0pt, partopsep=0pt, topsep=2pt}
\linespread{1.2}

\RequirePackage{minted}
\setminted{
    breaklines=true, fontsize=\small, frame=lines,
    numbers=none, style=bw, tabsize=4
}

\RequirePackage{xparse}
\NewDocumentCommand{\mil}{ O{text} m }{%
    \mintinline{#1}{#2}%
}

\RequirePackage{graphicx}
\RequirePackage{siunitx}

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
    \begin{tabular*}{\textwidth}{l l}
        \textbf{报告标题: } & \textbf{\academics@reporttitle} \\
        \textbf{日期: } & \academics@reportdate \\
        \textbf{姓名: } & \academics@authorname \\
        \textbf{学号: } & \academics@authorid \\
    \end{tabular*}\\
    \rule[2ex]{\textwidth}{2pt}
}
\makeatother
