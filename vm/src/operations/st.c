/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   st.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rschuppe <rschuppe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 20:15:01 by rschuppe          #+#    #+#             */
/*   Updated: 2019/03/22 20:53:44 by rschuppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int	op_st(t_env *env, t_carriage *carriage, int args_types, ...)
{
	va_list		args;
	int			value;
	int			mempos;

	va_start(args, args_types);
	if (get_reg_value(carriage, va_arg(args, int), &value, BIG_END))
	{
		if (ARG_TYPE(args_types, 1) == REG_CODE)
			set_reg_value(carriage, va_arg(args, int), value, BIG_END);
		else
			set_mem_value(env, carriage, va_arg(args, int), true, value);
	}
	va_end(args);
	return (-1);
}