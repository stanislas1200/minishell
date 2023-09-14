~ > replace by HOME but condition : no quote, exaclty ~ or ~/ and in export
cd should do ~ by herself ? : cd << EOF // need check in cd to
~
EOF

<!-- cd <<EOF -->
<!-- cd '/////' 2>/dev/null  -->
<!-- cd //  -->
cd '/etc' 
cd '/var' 
cd "doesntexist" 2>/dev/null 
<!-- echo ~ -->
<!-- echo '\" ' " \"\"" <!-- need interpret \ ?
echo \\\" \\\" \\\" \\\"\\\"\\\" \\\'\\\'\\\'
echo \$PWD
echo \-n hello -->
<!-- echo $12 <!-- if 1 is number only take 1 -->
<!-- /ls
./ls -->
<!-- /Users
Users -->
<!-- exit 0 0 -->
`<!-- cd print pwd -->

<!-- export t=~ && echo $t -->

<!-- export Dan+=2 ; export Dan+=2 ; export Da+=2 ; export Dann+=2 ; export lol ; export lol+=2 -->

<!-- export t=~/nimp -->

<!-- export lol+=2 puis export lol ne devrait pas changer la valeur de lol. -->

ctrl+c <<EOF > exit code 1

ctr+\ cat > quit

cat ctrl+c > code 130

<!-- echo ~/$USER$PWD -->
<!-- export dan=~ ; echo $dan -->

export | grep "OLDPWD"

lien symbolique ls -s file file

<!-- echo hey you | cat < Makefile > R | wc << EOF | <<EOF > G | > B echo hey | < R echo 1 > G 2 -->
echo hey <> FILE you ( should work and create FILE but not redirect )

echo hi >         ./outfiles/outfile01 bye // works but not in tester

<!-- builtin <<EOF
ctrl-d
segmentation fault  ./minishell -->
<!-- ctrl-c
exit minishell -->
<!-- export t="~" -->

==95496== Warning: noted but unhandled ioctl 0x2000746e with no size/direction hints.
==95496==    This could cause spurious value errors to appear.
==95496==    See README_MISSING_SYSCALL_OR_IOCTL for guidance on writing a proper wrapper.

// LEAKS
<!-- text not free in heredoc + redir -->

unset HOME
cd
==15345== Syscall param chdir(path) points to unaddressable byte(s)
==15345==    at 0x49B4A1B: chdir (syscall-template.S:78)
==15345==    by 0x10E2C8: cd2 (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==    by 0x10E277: cd (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==    by 0x10BB1D: execute_builtin (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==    by 0x10B73B: cmd_check_b (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==    by 0x10B994: execute_cmd (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==    by 0x10BA6E: execute_ast_node (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==    by 0x10C52D: process_parsing (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==    by 0x109808: main_loop (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==    by 0x10990E: main (in /mnt/c/Users/Stanislas/source/repos/MiniShell/minishell)
==15345==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==15345==

//HEREDOC
<!-- pwd <<eof <miss // should not do pwd -->

# fix this cat <<EOF <<EOF | echo 42 : heredoc pipe wait for EOF
no exactly like bash echo hey | cat | <<EOF 

# heredoc EOF NULL cmd not working
# heredoc not working after pipe

# Test 105: ❌ echo hi | >FILE echo bye >FILE-invalid_permission
# Only in ./bash_outfiles: outfile01
# mini outfiles:
# cat: './mini_outfiles/*': No such file or directory
# bash outfiles:
# mini exit code =
# bash exit code = 1

# echo hi | <<EOF ( should heredoc )
# echo 42 | <FILE | wc ( should try <FILE )

# Test  52: ❌  
# mini exit code = 2
# bash exit code = 0

# cat <<HERE | ls
# oi
# HERE

# cat <<HERE
# $USER
# oi
# HERE

# cat <minishell.h <<EOF <missing | ls (mini have pipe error)

# cat <minishell.h <<EOF | cat (should take heredoc as input and send it to next cat) //CHECK


# << echo oi
# > echo
# oi: command not found

# <<EOF // segfault , should work
# <<EOF $MISSIGN echo 42 // segfault , should work

# echo > R 42
# echo > R 42 > G 41
# > R echo 42 > G 41 > B 40

# > R echo 42 | <
# echo hey you | cat < TODO > R | wc << EOF | <<EOF > G | > B echo hey | < R echo 1 > G 2

Export: potentialy have "" for var data (USER="SGODIN")

# Syntax error: cmd | < cmd2 (only error if no cmd2)

# cat | cat << EOF ( heredoc broken if cmd that want read before | )

echo hey <> FILE you ( should work and create FILE but not redirect )

remake expand in 1 function for all cases (av, heredoc)
handle error in heredoc and executer

#invalid command, followed by empty variable, should clear the exit code // set last-exit = 0; on expand
doesntexist
$EMPTY
echo $?

# Neither of these unsets should break the shell, and you should still be able to call `/bin/ls`
unset USER
unset PATH
unset PWD
/bin/ls

--> DONE

# cat <minishell.h <<HERE <missing <<DOC | echo oi
# > HERE
# > DOC
# oi
# -bash: missing: No such file or directory

Test  17: ❌ echo hi |  "|" 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)

149/152 macos bash 3.2
133/151 linux bash 5.0.17(1)-release (x86_64-pc-linux-gnu)










SIGHUP (1): Hangup. Sent to a process when its controlling terminal is closed or disconnected.
SIGINT (2): Interrupt. Sent to a process when the user presses CTRL+C in the terminal.
SIGQUIT (3): Quit. Similar to SIGINT, but with a core dump.
SIGILL (4): Illegal instruction. Sent to a process when it attempts to execute an illegal instruction.
SIGABRT (6): Abort. Sent to a process to request abnormal termination, often used by the abort() function.
SIGFPE (8): Floating point exception. Sent to a process when a floating point arithmetic exception occurs.
SIGKILL (9): Kill. Sent to a process to forcefully terminate it, cannot be caught or ignored.
SIGSEGV (11): Segmentation fault. Sent to a process when it accesses a memory location that it is not allowed to access.
SIGPIPE (13): Broken pipe. Sent to a process when it writes to a pipe or socket that has been closed on the other end.
SIGALRM (14): Alarm clock. Sent to a process when the timer set by the alarm() function expires.
SIGTERM (15): Termination. Sent to a process to request termination, can be caught or ignored.
SIGUSR1 (10): User-defined signal 1.
SIGUSR2 (12): User-defined signal 2.
SIGCHLD (17): Child process terminated or stopped.
SIGCONT (18): Continue. Sent to a process to make it continue after being stopped (e.g., with SIGSTOP).
SIGSTOP (19): Stop. Sent to a process to pause its execution.
SIGTSTP (20): Terminal stop. Sent to a process when the user presses CTRL+Z in the terminal.
SIGTTIN (21): Background read from a terminal.
SIGTTOU (22): Background write to a terminal.

# echo hey you | cat < TODO > R | wc << EOF | <<EOF > G | > B echo | < R echo ARG > G ARG ARG
// Lexer Bulletproof
// parser probably Bulletproof 

// CHECK
heredoc ctrld -> -bash: warning: here-document at line 7 delimited by end-of-file (wanted `$LIMITER')
// FAILL linux
———————————— builtins
Test  18: ❌ echo "test 
mini output = (test)
bash output = ()
mini exit code = 0
bash exit code = 2
mini error = ()
bash error = ( unexpected EOF while looking for matching `"')
Test  28: ❌ echo test "" test "" test 
mini output = (test test test)
bash output = (test test test)
Test  40: ❌ cd $PWD hi 
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( too many arguments)
Test  50: ❌ exit hello 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test  56: ❌ cd 
mini output = (stan:/home/stan$ exit)
bash output = ()
mini exit code = stan:/home/stan$ exit
bash exit code = 0
Test  59: ❌ cd test_dir 
mini output = (stan:/mnt/c/Users/Stanislas/source/repos/MiniShell/minishell_tester/test_dir$ exit)
bash output = ()
mini exit code = stan:/mnt/c/Users/Stanislas/source/repos/MiniShell/minishell_tester/test_dir$ exit
bash exit code = 0
Test  65: ❌ cd .. 
mini output = (stan:/mnt/c/Users/Stanislas/source/repos/MiniShell$ exit)
bash output = ()
mini exit code = stan:/mnt/c/Users/Stanislas/source/repos/MiniShell$ exit
bash exit code = 0



———————————— redirects
———————————— extras
Test 136: ❌ ./test_files/invalid_permission 
mini exit code = 126
bash exit code = 126
Test 137: ❌ ./missing.out 
mini exit code = 126
bash exit code = 127
Test 138: ❌ missing.out 
mini exit code = 127
bash exit code = 127
———————————— pipes
———————————— nafuka
Test  12: ❌ echo "$$USER$USER""$USER" 
mini output = ($stan\stanstan)
bash output = (20062USER$USERstan)
Test  18: ❌ env | grep -v -E '^_=' | sort 
mini output = (DISPLAY=localhost:0.0 HOME=/home/stan HOSTTYPE=x86_64 LANG=C.UTF-8 LESSCLOSE=/usr/bin/lesspipe %s %s LESSOPEN=| /usr/bin/lesspipe %s LOGNAME=stan LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36: MOTD_SHOWN=update-motd NAME=LAPTOP-5T5FAU1D OLDPWD=/mnt/c/Users/Stanislas/source/repos/MiniShell PATH=/home/stan/.local/bin:/home/stan/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Program Files (x86)/Common Files/Oracle/Java/javapath:/mnt/c/Python39/Scripts/:/mnt/c/Python39/:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/:/mnt/c/WINDOWS/System32/OpenSSH/:/mnt/c/ProgramData/chocolatey/bin:/mnt/c/Program Files (x86)/QuickTime/QTSystem/:/mnt/c/Program Files/Git/cmd:/mnt/c/Program Files/php-8.1.3-Win32-vs16-x64:/mnt/c/Program Files/dotnet/:/mnt/c/msys64/mingw64/bin:/mnt/c/Program Files/nodejs/:/mnt/c/Users/Stanislas/AppData/Local/Microsoft/WindowsApps:/mnt/c/Program Files (x86)/heroku/bin:/mnt/c/Users/Stanislas/AppData/Local/GitHubDesktop/bin:/mnt/c/Users/Stanislas/.dotnet/tools:/mnt/c/Users/Stanislas/AppData/Local/Programs/Microsoft VS Code/bin:/mnt/c/Users/Stanislas/AppData/Roaming/npm:/snap/bin PWD=/mnt/c/Users/Stanislas/source/repos/MiniShell/minishell_tester SHELL=/bin/bash SHLVL=2 TERM=xterm-256color USER=stan WSLENV= WSL_DISTRO_NAME=Ubuntu WSL_INTEROP=/run/WSL/8_interop XDG_DATA_DIRS=/usr/local/share:/usr/share:/var/lib/snapd/desktop)
bash output = (DISPLAY=localhost:0.0 HOME=/home/stan HOSTTYPE=x86_64 LANG=C.UTF-8 LESSCLOSE=/usr/bin/lesspipe %s %s LESSOPEN=| /usr/bin/lesspipe %s LOGNAME=stan LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36: MOTD_SHOWN=update-motd NAME=LAPTOP-5T5FAU1D OLDPWD=/mnt/c/Users/Stanislas/source/repos/MiniShell PATH=/home/stan/.local/bin:/home/stan/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Program Files (x86)/Common Files/Oracle/Java/javapath:/mnt/c/Python39/Scripts/:/mnt/c/Python39/:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/:/mnt/c/WINDOWS/System32/OpenSSH/:/mnt/c/ProgramData/chocolatey/bin:/mnt/c/Program Files (x86)/QuickTime/QTSystem/:/mnt/c/Program Files/Git/cmd:/mnt/c/Program Files/php-8.1.3-Win32-vs16-x64:/mnt/c/Program Files/dotnet/:/mnt/c/msys64/mingw64/bin:/mnt/c/Program Files/nodejs/:/mnt/c/Users/Stanislas/AppData/Local/Microsoft/WindowsApps:/mnt/c/Program Files (x86)/heroku/bin:/mnt/c/Users/Stanislas/AppData/Local/GitHubDesktop/bin:/mnt/c/Users/Stanislas/.dotnet/tools:/mnt/c/Users/Stanislas/AppData/Local/Programs/Microsoft VS Code/bin:/mnt/c/Users/Stanislas/AppData/Roaming/npm:/snap/bin PWD=/mnt/c/Users/Stanislas/source/repos/MiniShell/minishell_tester SHELL=/bin/bash SHLVL=1 TERM=xterm-256color USER=stan WSLENV= WSL_DISTRO_NAME=Ubuntu WSL_INTEROP=/run/WSL/8_interop XDG_DATA_DIRS=/usr/local/share:/usr/share:/var/lib/snapd/desktop)
Test  32: ❌ exit -- 42 
mini exit code = 1
bash exit code = 42
mini error = ( too many arguments)
bash error = ()
mini exit code = 1
bash exit code = 2
mini error = ( too many arguments)
bash error = ( numeric argument required)
Test  34: ❌ exit -- -- 42 
mini exit code = 1
bash exit code = 2
mini error = ( too many arguments)
bash error = ( numeric argument required)
Test  35: ❌ exit --42 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test  36: ❌ exit ++42 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test  38: ❌ exit a 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test  39: ❌ exit 1a 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test  40: ❌ exit + 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test  41: ❌ exit - 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test  42: ❌ exit "          " 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test  62: ❌ export TEST="echo hello"| $TEST 
mini exit code = 2
bash exit code = 0
mini error = ( syntax error near unexpected token `end of file')
bash error = ()
Test  65: ❌ export TEST="echo hello"| export TEST2="$TEST"| $TEST2 
mini exit code = 2
bash exit code = 0
mini error = ( syntax error near unexpected token `end of file')
bash error = ()
Test  26: ❌ export PATH=":a:b"| cmd 
mini exit code = 1
bash exit code = 127
mini error = ( Exec format error)
bash error = ( command not found)
Test  13: ❌ export=/ 
mini exit code = 1
bash exit code = 0
mini error = ( No such file or directory)
bash error = ()
Test  18: ❌ echo hello > out1.txt > out2.txt > out3.txt 2> err1.txt 2> err2.txt 2> err3.txt| cat out1.txt out2.txt out3.txt err1.txt err2.txt err3.txt 
mini output = ()
bash output = (hello 2 2 2)
Test  19: ❌ echo hello >> out1.txt >> out2.txt >> out3.txt 2>> err1.txt 2>> err2.txt 2>> err3.txt| cat out1.txt out2.txt out3.txt err1.txt err2.txt err3.txt 
mini output = (hello hello 2 2 2)
bash output = (hello hello 2 2 2 hello 2 2 2 hello 2 2 2)
Test  20: ❌ ls > out1.txt > out2.txt > out3.txt 2> err1.txt 2> err2.txt 2> err3.txt| cat out1.txt out2.txt out3.txt err1.txt err2.txt err3.txt 
mini output = (hello hello hello hello 2 2 2 hello 2 2 2 hello 2 2 2)
bash output = ()
mini error = ( No such file or directory No such file or directory No such file or directory)
bash error = ()
Test  22: ❌ ls no_such_file > out1.txt > out2.txt > out3.txt 2> err1.txt 2> err2.txt 2> err3.txt| cat out1.txt out2.txt out3.txt err1.txt err2.txt err3.txt 
 wildcards) lol loop.out ls manual_tests mini_outfiles os_specific out out1.txt out2.txt out3.txt outfiles pipes redirects syntax test test_dir test_files tester testfile testfile
bash output = ()
mini error = ( No such file or directory No such file or directory No such file or directory No such file or directory)
bash error = ()
Test  25: ❌ echo a > out| echo b >> out| echo c >> out| echo d > out| cat out 
mini output = ()
bash output = (b)
Test  26: ❌ echo hello 2147483647> file 
mini output = (hello 2147483647)
bash output = ()
mini exit code = 0
bash exit code = 1
mini error = ()
bash error = ( Bad file descriptor)
Test  27: ❌ echo hello 2147483648> file 
mini output = (hello 2147483648)
bash output = ()
Test  28: ❌ echo a > $NO_ENV | echo b 
mini output = ()
bash output = (b)
mini exit code = 2
bash exit code = 0
mini error = ( syntax error near unexpected token `end of file')
bash error = ( ambiguous redirect)

loop: work I think
echo a > f| echo b >> f| cat f f f >> ff| cat ff ff >> f| cat f

Test  26: ❌ export SHLVL=`printf "tnvfr 42"` 
mini exit code = 1
bash exit code = 0
mini error = ( not a valid identifier)
bash error = ()
Test  28: ❌ export SHLVL=`printf " 42 "` 
mini exit code = 1
bash exit code = 0
mini error = ( not a valid identifier)
bash error = ()
Test  56: ❌ no_such_file 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test  57: ❌ ./no_such_file 
mini exit code = 126
bash exit code = 127
Test  58: ❌ /bin/ls/no_such_file 
mini exit code = 1
bash exit code = 126
Test  59: ❌ /bin/no_such_dir/file 
mini exit code = 1
bash exit code = 127
Test  60: ❌ perm 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test  62: ❌ ./perm 
mini exit code = 126
bash exit code = 0
mini error = ( Exec format error)
bash error = ()
Test  61: ✅ touch perm| chmod 000 perm 
Test  62: ❌ ./perm 
mini exit code = 126
bash exit code = 0
mini error = ( Exec format error)
bash error = ()
Test  63: ✅ touch perm| chmod 000 perm 
Test  64: ❌ perm 
mini exit code = 1
bash exit code = 127
mini error = ( Exec format error)
bash error = ( command not found)
Test  65: ✅ touch perm| chmod 100 perm 
Test  66: ❌ ./perm 
mini exit code = 126
bash exit code = 0
mini error = ( Exec format error)
bash error = ()
Test  67: ✅ touch perm| chmod 100 perm 
Test  68: ❌ perm 
mini exit code = 1
bash exit code = 127
mini error = ( Exec format error)
bash error = ( command not found)
Test  69: ✅ touch perm| chmod 300 perm 
Test  70: ❌ ./perm 
mini exit code = 126
bash exit code = 0
mini error = ( Exec format error)
bash error = ()
Test  71: ✅ touch perm| chmod 300 perm 
Test  72: ❌ perm 
mini exit code = 1
bash exit code = 127
mini error = ( Exec format error)
bash error = ( command not found)
Test  73: ✅ touch perm| chmod 500 perm 
Test  74: ❌ ./perm 
mini exit code = 126
bash exit code = 0
mini error = ( Exec format error)
bash error = ()
Test  75: ✅ touch perm| chmod 500 perm 
Test  76: ❌ ./empty 
mini exit code = 126
bash exit code = 127
Test  82: ❌ ./exe 
mini exit code = 126
bash exit code = 127
Test  84: ❌ echo a | $NO_ENV | cat 
mini exit code = 2
bash exit code = 0
mini error = ( syntax error near unexpected token `end of file')
bash error = ()
--------------- thallard
Test   3: ❌ eCho 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test   4: ❌ eChO 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test   5: ❌ eCHO 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test   6: ❌ ECHO 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test  12: ❌ echo -n bonjour 
mini output = ()
bash output = (bonjour)
Test  13: ❌ echo -nn bonjour 
mini output = ()
bash output = (bonjour)
Test  14: ❌ echo -n -n -n bonjour 
mini output = ()
bash output = (bonjour)
Test  15: ❌ echo -n -n -n bonjour 
mini output = ()
bash output = (bonjour)
Test  16: ❌ echo "-n" bonjour 
mini output = ()
bash output = (bonjour)
Test  18: ❌ echo "-nnnn" bonjour 
mini output = ()
bash output = (bonjour)
Test  39: ❌ echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER '' 
mini output = (stan$USERtext oui oui oui oui stan oui stan)
bash output = (stan$USERtext oui oui oui oui stan oui stan )
Test  40: ❌ echo $USER '' $USER $USER '' $USER '' $USER -n $USER 
mini output = (stan stan stan stan stan -n stan)
bash output = (stan stan stan stan stan -n stan)
Test  43: ❌ echo " " " """ ''' 
mini output = (\ \ \ \\\ \\\)
bash output = (" " " """ ''')
Test  78: ❌ hello world 
mini exit code = 1
bash exit code = 127
mini error = ( Permission denied)
bash error = ( command not found)
Test  87: ❌ ~ 
mini exit code = 1
bash exit code = 126
mini error = ( Permission denied)
bash error = ( Is a directory)
Test  89: ❌ /Users 
mini exit code = 1
bash exit code = 127
Test  91: ❌ . 
mini exit code = 126
bash exit code = 2
mini error = ( Permission denied)
bash error = ( filename argument required)
Test  92: ❌ .. 
mini exit code = 126
bash exit code = 127
mini error = ( Permission denied)
bash error = ( command not found)
Test  93: ❌ / 
mini exit code = 1
bash exit code = 126
mini error = ( Permission denied)
bash error = ( Is a directory)
Test  95: ❌ .echo. 
mini exit code = 126
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 108: ❌ echo -n oui 
mini output = ()
bash output = (oui)
Test 109: ❌ trying to destroy your minishell 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 110: ❌ trying something again echo if you see this message thats not a good new 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 111: ❌ qewew 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 112: ❌ wtf 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 113: ❌ hi im thallard 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 115: ❌ your minishell is perfect 
mini exit code = 1
bash exit code = 127
mini error = ( No such file or directory)
bash error = ( command not found)
Test 123: ❌ exit --1239312 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 124: ❌ exit ++++1203020103 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 126: ❌ exit ++++++0 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 127: ❌ exit -----0 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 128: ❌ exit azerty 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 129: ❌ exit kewkwqke 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 130: ❌ exit a 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 131: ❌ exit z 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 136: ❌ exit "+++1230" 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 139: ❌ exit "'42'" 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 144: ❌ exit 9223372036854775808 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 146: ❌ exit wrong 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 147: ❌ exit wrong_command 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 148: ❌ exit something 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 153: ❌ exit --000 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 154: ❌ exit +++++++000 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 155: ❌ exit ++++3193912939 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 156: ❌ exit ---31232103012 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 157: ❌ exit "something" 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 158: ❌ exit q 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 159: ❌ exit help 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 160: ❌ exit export 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 161: ❌ exit echo 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 163: ❌ exit something somethingv2 
mini exit code = 1
bash exit code = 2
mini error = ( too many arguments)
bash error = ( numeric argument required)
Test 165: ❌ exit echo something 
mini exit code = 1
bash exit code = 2
mini error = ( too many arguments)
bash error = ( numeric argument required)
Test 166: ❌ exit exit 
mini exit code = 255
bash exit code = 2
mini error = ( numeric argument required)
bash error = ( numeric argument required)
Test 176: ❌ export --TEST=123 
mini exit code = 1
bash exit code = 2
mini error = ( not a valid identifier)
bash error = ( invalid option)
Test 181: ❌ export TE\ST=100 
mini exit code = 1
bash exit code = 0
mini error = ( not a valid identifier)
bash error = ()
Test 197: ❌ export TES\T 
mini exit code = 1
bash exit code = 0
mini error = ( not a valid identifier)
bash error = ( not a valid identifier)
Test 204: ❌ export -TEST=123 
mini exit code = 1
bash exit code = 2
mini error = ( not a valid identifier)
bash error = ( invalid option)
Test 227: ❌ env | grep "SHLVL" 
mini output = (SHLVL=2)
bash output = (SHLVL=1)
Test 259: ❌ unset TES\T 
mini exit code = 1
bash exit code = 0
mini error = ( not a valid identifier)
bash error = ( not a valid identifier)
Test 267: ❌ unset -TEST 
mini exit code = 1
bash exit code = 2
mini error = ( not a valid identifier)
bash error = ( invalid option)