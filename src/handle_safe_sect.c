/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_safe_sect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchicote <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 17:57:21 by cchicote          #+#    #+#             */
/*   Updated: 2019/02/16 17:57:38 by cchicote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int							check_sect_offset_file(t_file *file,
								uint32_t offset)
{
	return (file->curr_arch + offset <= file->content + file->len);
}

int							check_sect_offset_header(t_file *file,
								uint32_t offset)
{
	return (file->curr_arch + offset >= file->curr_header_end);
}

int							is_filetype_dylib(uint32_t filetype)
{
	return (filetype == MH_DYLIB || filetype == MH_DYLIB_STUB);
}

int							check_section_32(t_file *file, struct section sect,
								uint32_t sect_index, uint32_t seg_index)
{
	if (sect.flags & S_ZEROFILL)
		return (EXIT_SUCCESS);
	if (!check_sect_offset_file(file, sect.offset))
	{
		ft_strcmp(file->command, "ft_nm") == 0
			? perror_nm_trunc_malf_sect_file(file->name, sect_index,
			seg_index, "LC_SEGMENT")
			: perror_otool_trunc_malf_sect_file(file->name,
			sect_index, seg_index, "LC_SEGMENT");
		return (EXIT_FAILURE);
	}
	if (!check_sect_offset_header(file, sect.offset)
		&& !is_filetype_dylib(file->curr_filetype))
	{
		ft_strcmp(file->command, "ft_nm") == 0
			? perror_nm_trunc_malf_sect_header(file->name, sect_index,
			seg_index, "LC_SEGMENT")
			: perror_otool_trunc_malf_sect_header(file->name,
			sect_index, seg_index, "LC_SEGMENT");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int							check_section_64(t_file *file,
								struct section_64 sect, uint32_t sect_index,
								uint32_t seg_index)
{
	if (sect.flags & S_ZEROFILL)
		return (EXIT_SUCCESS);
	if (!check_sect_offset_file(file, sect.offset))
	{
		ft_strcmp(file->command, "ft_nm") == 0
			? perror_nm_trunc_malf_sect_file(file->name, sect_index,
			seg_index, "LC_SEGMENT_64")
			: perror_otool_trunc_malf_sect_file(file->name,
			sect_index, seg_index, "LC_SEGMENT_64");
		return (EXIT_FAILURE);
	}
	if (!check_sect_offset_header(file, sect.offset)
		&& !is_filetype_dylib(file->curr_filetype))
	{
		ft_strcmp(file->command, "ft_nm") == 0
			? perror_nm_trunc_malf_sect_header(file->name, sect_index,
			seg_index, "LC_SEGMENT_64")
			: perror_otool_trunc_malf_sect_header(file->name,
			sect_index, seg_index, "LC_SEGMENT_64");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
