#include "minishell.h"

void	strip_quotes(char *src, char *dest)
{
	int	i;
	int	j;
	int	quote;

	i = -1;
	j = 0;
	quote = 0;
	while (src[++i])
	{
		if ((src[i] == '\'' || src[i] == '\"') && quote == 0)
			quote = src[i];
		else if (src[i] == quote)
			quote = 0;
		else
			dest[j++] = src[i];
	}
	dest[j] = '\0';
}

void	get_exit_code(char **result, t_data *data)
{
	char	*tmp;
	char	*tmp2;

	tmp = *result;
	tmp2 = ft_itoa(data->last_exit);
	if (!tmp2)
		return ;
	*result = ft_strjoin(*result, tmp2);
	free(tmp2);
	free(tmp);
	data->i++;
}

int	expand(t_data *data, char **result, char *input, int var_start)
{
	char	*var_name;
	char	*var_value;

	while (ft_isalnum(input[data->i]))
		data->i++;
	if (input[var_start] == '?')
		get_exit_code(result, data);
	else if (data->i > var_start)
	{
		var_name = malloc(data->i - var_start + 1);
		if (!var_name)
			return (1);
		ft_strlcpy(var_name, input + var_start, data->i - var_start + 1);
		var_name[data->i - var_start] = '\0';
		var_value = ft_getenv(data->env, var_name);
		if (var_value)
			*result = free_join(*result, var_value);
		free(var_name);
	}
	else
		*result = free_join(*result, "$");
	if (!*result)
		return (1);
	return (0);
}

int	result_add(t_data *data, char **result, char *input, char *tmp)
{
	tmp = ft_calloc(ft_strlen(*result) + 2, sizeof(char));
	if (!tmp)
		return (free(*result), 1);
	ft_strlcat(tmp, *result, ft_strlen(*result) + 2);
	ft_strlcat(tmp, input + data->i, ft_strlen(*result) + 2);
	free(*result);
	*result = tmp;
	data->i++;
	return (0);
}

char	*expand_variables(char *input, t_data *data)
{
	char	*result;
	char	*tmp;
	int		quote;

	quote = 0;
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	data->i = 0;
	while (input[data->i])
	{
		if ((input[data->i] == '\'' || input[data->i] == '\"') && quote == 0)
			quote = input[data->i];
		else if (input[data->i] == quote)
			quote = 0;
		if (input[data->i] == '$' && quote != '\'')
		{
			data->i++;
			if (expand(data, &result, input, data->i))
				return (free(result), NULL);
		}
		else if (result_add(data, &result, input, tmp))
			return (NULL);
	}
	return (result);
}
