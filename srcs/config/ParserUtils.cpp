/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 20:34:19 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/25 20:49:24 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParserUtils.hpp"

//AQUI que se imprime bien feo
std::string	directiveToString(const s_directive& directive, int lvl)
{
	std::ostringstream	out;
	std::string	indent(lvl, '\t');
	std::string	childIndent(lvl + 1, '\t');

	out << indent << "Direct name: " << directive.name << "\n";
	out << indent << "Arguments: " << vectorToString(directive.args) << "\n";
	out << indent << "Chidlren:\n";
	out << indent << "{\n";
	
	if (directive.children.empty())
		out << childIndent << "No have any.\n";
	else
	{
		for (std::vector<t_directive>::const_iterator vit = directive.children.begin();
				vit != directive.children.end(); vit++)
			out << directiveToString(*vit, lvl + 1);
	}

	out << indent << "}\n";

	return (out.str());
}
