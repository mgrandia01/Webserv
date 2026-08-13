/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 13:06:43 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/12 14:28:35 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <vector>
# include <iostream>
# include <sstream>

/* ********************************** enums ********************************* */
typedef enum	e_methods
{
	GET = 0,
	POST,
	DELETE
}	t_methods;

/* ********************************* structs ******************************** */

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
	std::string			errorFile;
}	t_errorPage;

typedef struct	s_return
{
	bool		isEnabled;
	int			code;
	std::string	target;
}	t_return;

/* ******************************** templates ******************************* */

template<typename T>
std::string	vectorToString(const std::vector<T>& vec)
{
	std::ostringstream	outstr;

	outstr << "[";
	for (typename std::vector<T>::const_iterator vit = vec.begin();
			vit != vec.end(); vit++)
	{
		if (vit != vec.begin())
			outstr << ", ";
		outstr << *vit;
	}
	outstr << "]";
	return (outstr.str());
}

template <typename T>
std::string intToString(const T& value)
{
	std::ostringstream outstr;

	outstr << value;
	return (outstr.str());
}
#endif
