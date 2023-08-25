# include "minishell.h"




/* DEBUG */
void	lexer_print(t_lexer *lexer)
{
	t_token	*token;

	token = lexer->tokens;
	printf(Y "lexer:\n" C);
	while (token)
	{
		if (token->type == CHAR_NULL)
			printf("type: %d, value: %s|\n", token->type, "NULL");
		else
			printf("type: %d, value: %s|\n", token->type, token->data);
		token = token->next;
	}
	printf("\n");
}

/* Debug */

void	print_ast_node(t_ASTNode *node, int indent)
{
	if (node == NULL)
		return ;

	// Print the current node's data with indentation
	for (int i = 0; i < indent; i++)
		printf("    ");

	printf("Type: %d, Data: %s\n", node->type, node->data);

	// Recursively print left and right subtrees
	print_ast_node(node->left, indent + 1);
	print_ast_node(node->right, indent + 1);
}

void print_ast_tree_vertical(t_ASTNode *node, int level) {
	if (node != NULL) {
		print_ast_tree_vertical(node->right, level + 1);

		for (int i = 0; i < level; i++) {
			printf("    ");
		}
		printf(M "|-- " C);
		if (node->type == CHAR_PIPE)
			printf(Y "%s" C, node->data);
		else if ( node->type == CHAR_INPUTR || node->type == CHAR_OUTPUTR | node->type == 3)
			printf(G "%c " C, node->type == 3 ? 'A' : node->type);
		else if (node->type == 4)
			printf(R "H " C);
		printf("%s\n", node->data);

		print_ast_tree_vertical(node->left, level + 1);
	}
}

void	print_ast(t_ASTNode *root)
{
	if (root == NULL)
		return (printf("Empty tree\n"), (void)NULL);
	printf(Y "Abstract Syntax Tree:\n" C);
	print_ast_node(root, 0);
	printf("\n");
	print_ast_tree_vertical(root, 0);
}

/* CHECK  AFTER REDIRECTION */