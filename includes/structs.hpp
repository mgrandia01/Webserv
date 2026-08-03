/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 13:06:43 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/03 18:46:27 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_HPP
# define STRUCTS_HPP

typedef enum	e_methods
{
	GET = 0,
	POST,
	DELETE
}	t_methods;

typedef struct	s_directive
{
	bool						isBlock;
	std::string					name;
	std::vector<std::string>	args;
	std::vector<s_directive>	children;
}	t_directive;

typedef struct	s_errorPage
{
	std::vector<int>	codes;
	std::string			src;
}	t_errorPage;

typedef struct	s_return
{
	bool		isEnabled;
	int			code;
	std::string	target;
}	t_return;

#endif
