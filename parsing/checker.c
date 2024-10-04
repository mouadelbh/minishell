#include "parsing.h"

int	check_dollar(char *s, int i)
{
	if (!s[i + 1] || isspace(s[i + 1]) || s[i + 1] == '$' || quotes_open(s, i) == 1)
		return (1);
	if ((s[i + 1] == '\"' || s[i + 1] == '\'') && quotes_open(s, i))
		return (1);
	return (0);
}

int	check_token(int c)
{
	if (c == '|')
		return (1);
	if (c == '<' || c == '>')
		return (2);
	return (0);
}

int	checkspaces(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	while (is_space(line[i]))
		i++;
	if (!line[i])
		return (0);
	return (1);
}

int	checkquotes(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && !check_dollar(line ,i))
			line[i] = -1;
		i++;
	}
	if (quotes_open(line, i) == 1)
		return (ft_putstr_fd("minishell: syntax error near unexpected token '\''\n", 2),0);
	if (quotes_open(line, i) == 2)
		return (ft_putstr_fd("minishell: syntax error near unexpected token '\"'\n", 2),0);
	return (1);
}
