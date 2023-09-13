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

# cat <minishell.h <<EOF | cat (should take heredoc as input and send it to next cat)


# << echo oi
# > echo
# oi: command not found

# <<EOF // segfault , should work
# <<EOF $MISSIGN echo 42 // segfault , should work

# echo > R 42
# echo > R 42 > G 41
# > R echo 42 > G 41 > B 40

# > R echo 42 | <
#echo hey you | cat < TODO > R | wc << EOF | <<EOF > G | > B echo hey | < R echo 1 > G 2

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