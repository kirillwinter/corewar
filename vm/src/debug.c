/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rschuppe <rschuppe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 16:37:12 by rschuppe          #+#    #+#             */
/*   Updated: 2019/03/27 14:33:56 by rschuppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	print_champion(t_champion *champion)
{
	ft_printf("\nChamp:\nName: %s\nComment: %s\nSize exec: %u\n",
		champion->prog_name, champion->comment, champion->prog_size);
	print_memory(champion->exec_code, champion->prog_size);
	ft_putchar('\n');
}

void	print_carriage(t_env *env, t_carriage *carriage)
{
	int i;
	int val;

	ft_printf("Carriage %d:\n", carriage->id);
	ft_printf("Position: %d, byte: ", carriage->position);
	print_memory(env->field + carriage->position, 1);
	ft_printf("\nLast_live_cycle: %d\n", carriage->last_live_cycle);
	ft_printf("Carry: %d\n", carriage->carry);
	ft_printf("Registers:\n");
	i = 0;
	while (i < REG_NUMBER)
	{
		val = carriage->registers[i];
		swap_bytes(&val, sizeof(int));
		ft_printf("[%02d] = %10d, ", i + 1, val);
		print_memory(&val, REG_SIZE);
		write(1, "\n", 1);
		i++;
	}
}

void	print_do_op(int carriage_id, char *op_cmd, t_arg *args, int8_t args_count)
{
	int i;

	ft_printf("P%5d | %s", carriage_id, op_cmd);
	i = 0;
	while (i < args_count)
	{
		write(1, " ", 1);
		if (args[i].type == REG_CODE)
		{
			write(1, "r", 1);
			ft_putnbr(args[i].value);
		}
		else if (args[i].type == DIR_CODE)
		{
			ft_putnbr(args[i].content);
		}
		else
		{
			ft_putnbr(args[i].value);
		}
		i++;
	}
}

int		print_move(t_env *env, unsigned int curpos, unsigned int len)
{
	if (curpos)
		ft_printf("ADV %d (%#06x -> %#06x) ", len, curpos, curpos + len);
	else
		ft_printf("ADV %d (0x%04x -> %#06x) ", len, curpos, curpos + len);
	while (len)
	{
		ft_printf("%02x ", *(env->field + curpos % MEM_SIZE));
		curpos++;
		len--;
	}
	write(1, "\n", 1);
	// if (curpos + len == 0x0f7f)	//	DEBUG
	// {
	// 	print_memory(env->field + curpos + len, 6);
	// 	write(1, "\n", 1);
	// }
	return (1);
}
