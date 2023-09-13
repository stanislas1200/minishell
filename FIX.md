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

<!-- ls.c > doit regarder d'abord dans PATH -->

<!-- ctrl+c <<EOF > exit code 1 -->

<!-- ctr+\ cat > quit -->

<!-- cat ctrl+c > code 130 -->

exit "        42           "


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