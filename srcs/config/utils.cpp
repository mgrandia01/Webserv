/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcuenca- <mcuenca-@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 20:34:19 by mcuenca-          #+#    #+#             */
/*   Updated: 2026/08/19 17:00:22 by mcuenca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

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

/*
	std::ostringstream	out;

	if (lvl < 1)
		out << std::endl;
	
	for (int i = 0; i < lvl; i++)
		out << "\t";
	out << "Directive name: " << directive.name << std::endl;
	for (int i = 0; i < lvl; i++)
		out << "\t";
	out << "Arguments: " << vectorToString(directive.args) << std::endl;
	
	std::vector<std::string>	children;

	for (std::vector<t_directive>::const_iterator vit = directive.children.begin();
			vit != directive.children.end(); vit++)
	{
		std::string tmp;

		//for (int i = 0; i < lvl; i++)
		//	tmp += "\t";
		tmp = directiveToString(*vit, lvl + 1);
		children.push_back(tmp);
	}
	out << "Children: " << std::endl;
	for (int i = 0; i < lvl; i++)
		out << "\t";
	out << "{" << std::endl;
	for (int i = 0; i < lvl; i++)
		out << "\t";
	out << vectorToString(children);
	out << "}" << std::endl;

	return (out.str());*/
}
