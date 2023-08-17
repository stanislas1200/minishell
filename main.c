/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 16:39:14 by sgodin            #+#    #+#             */
/*   Updated: 2023/08/17 15:46:15 by sgodin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "minishell.h"

void	print_header(void)
{
	printf("\n");
	printf(G "********************************************\n");
	printf("*                                          *\n");
	printf("*        " M "Welcome to My Custom Shell!" R "       *\n");
	printf("*                                          *\n");
	printf("********************************************\n" C);
	printf("\n");
}

void	print_exit_message(void)
{
	printf(R "Error: Shell has encountered an   \n");
	printf("                   		unexpected shutdown!\n");
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		// printf("\33[2K\r");
		// rl_replace_line("", 0);
		// rl_on_new_line();
		// rl_redisplay();
		rl_forced_update_display(); // work for ctrl-C but not for ctr-L
	}
}

void	*get_prompt(void)
{
	char		*str;
	char		cwd[1024];
	char		*user;

	str = malloc(1024);
	if (!str)
		return (NULL);
	str[0] = '\0';
	user = getenv("USER");
	if (!getcwd(cwd, sizeof(cwd)) || !user)
	{
		cperror("getcwd", NULL, 1);
		free(str);
		return (NULL);
	}
	strcat(str, "\x1b[1;32m");
	strcat(str, getenv("USER"));
	strcat(str, "\x1b[0m:\x1b[1;36m");
	strcat(str, cwd);
	strcat(str, "\x1b[0m$ \0");
	return (str);
}

int	main(void)
{
	char		*buff;
	char		*prompt;
	t_lexer		*lexer;
	t_ASTNode	*ast_root;

	// envp = dup_env(__environ);
	// update_pwd(&envp);
	signal(SIGINT, signal_handler);
	print_header();
	while (1)
	{
		prompt = get_prompt();
		buff = readline(prompt);
		if (!buff)
		{
			printf("\n");
			print_exit_message();
			break ;
		}
		free(prompt);
		add_history(buff);
		lexer = lexer_build(buff);
		lexer_print(lexer); /* DEBUG */
		if (lexer)
			ast_root = parse(lexer);
		print_ast(ast_root);	/* DEBUG */
		execute_ast_node(ast_root);
		// eval(buff);
	}
	return (0);
}
