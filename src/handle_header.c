/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_header.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cchicote <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 17:01:49 by cchicote          #+#    #+#             */
/*   Updated: 2019/01/11 17:02:01 by cchicote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*uint64_t		swap_endian(uint64_t x)
{
	return (((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8)\
		| (((x) & 0xff00) << 8) | (((x) & 0xff) << 24)));
}*/

unsigned char				*swap_endian(unsigned char *data, size_t n)
{
	size_t					i;
	size_t					half;
	unsigned char			swap;

	i = 0;
	half = n / 2;
	while (i < half)
	{
		swap = data[i];
		data[i] = data[n - i - 1];
		data[n - i - 1] = swap;
		++i;
	}
	return (data);
}

void						swap_fat_arch(t_file *file, uint32_t offset)
{
	struct fat_arch			*fat_arch;

	fat_arch = (struct fat_arch*)file->content + offset;
	SWAP(fat_arch->cputype);
	SWAP(fat_arch->cpusubtype);
	SWAP(fat_arch->offset);
	SWAP(fat_arch->size);
	SWAP(fat_arch->align);
}

void						swap_32_header(t_file *file, uint32_t offset)
{
	struct mach_header			*mach_header;

	mach_header = (struct mach_header*)file->content + offset;
	SWAP(mach_header->magic);
	SWAP(mach_header->cputype);
	SWAP(mach_header->cpusubtype);
	SWAP(mach_header->filetype);
	SWAP(mach_header->ncmds);
	SWAP(mach_header->sizeofcmds);
	SWAP(mach_header->flags);
}

void						swap_64_header(t_file *file, uint32_t offset)
{
	struct mach_header_64			*mach_header_64;
	
	mach_header_64 = (struct mach_header_64*)file->content + offset;
	SWAP(mach_header_64->magic);
	SWAP(mach_header_64->cputype);
	SWAP(mach_header_64->cpusubtype);
	SWAP(mach_header_64->filetype);
	SWAP(mach_header_64->ncmds);
	SWAP(mach_header_64->sizeofcmds);
	SWAP(mach_header_64->flags);
	SWAP(mach_header_64->reserved);
}

void						swap_load_command(t_file *file, uint32_t offset)
{
	struct load_command		*load_command;

	load_command = (struct load_command*)file->content + offset;
	SWAP(load_command->cmd);
	SWAP(load_command->cmdsize);
}

void						swap_32_segment_command(t_file *file, uint32_t offset)
{
	struct segment_command	*segment_command;

	segment_command = (struct segment_command*)file->content + offset;
	SWAP(segment_command->cmd);
	SWAP(segment_command->cmdsize);
	SWAP(segment_command->segname);
	SWAP(segment_command->vmaddr);
	SWAP(segment_command->vmsize);
	SWAP(segment_command->fileoff);
	SWAP(segment_command->filesize);
	SWAP(segment_command->maxprot);
	SWAP(segment_command->initprot);
	SWAP(segment_command->nsects);
	SWAP(segment_command->flags);
}

void							swap_64_segment_command(t_file *file, uint32_t offset)
{
	struct segment_command_64	*segment_command_64;

	segment_command_64 = (struct segment_command_64*)file->content + offset;
	SWAP(segment_command_64->cmd);
	SWAP(segment_command_64->cmdsize);
	SWAP(segment_command_64->segname);
	SWAP(segment_command_64->vmaddr);
	SWAP(segment_command_64->vmsize);
	SWAP(segment_command_64->fileoff);
	SWAP(segment_command_64->filesize);
	SWAP(segment_command_64->maxprot);
	SWAP(segment_command_64->initprot);
	SWAP(segment_command_64->nsects);
	SWAP(segment_command_64->flags);
}

void						swap_symtab_command(t_file *file, uint32_t offset)
{
	struct symtab_command	*symtab_command;

	symtab_command = (struct symtab_command*)file->content + offset;
	SWAP(symtab_command->cmd);
	SWAP(symtab_command->cmdsize);
	SWAP(symtab_command->symoff);
	SWAP(symtab_command->nsyms);
	SWAP(symtab_command->stroff);
	SWAP(symtab_command->strsize);
}

void						swap_fat_header(t_file *file, uint32_t offset)
{
	t_fh		*fat_header;

	fat_header = (t_fh*)file->content + offset;
	SWAP(fat_header->magic);
    SWAP(fat_header->nfat_arch);
}

void						handle_fat_header(t_file *file)
{
	struct fat_header		*fat_header;
	struct fat_arch			*fat_arch;
	uint32_t				i;
	uint32_t				narch;

	i = -1;
	if (file->is_little_endian)
	{
		swap_fat_header(file, 0);
		printf("A\n");
		swap_fat_arch(file, sizeof(struct fat_header));
	}
	fat_header = (struct fat_header*)file->content;
	narch = fat_header->nfat_arch;
	fat_arch = (void*)file->content + sizeof(struct fat_header);
	while (++i < narch)
	{
		if ((cpu_type_t)fat_arch[i].cputype == CPU_TYPE_X86_64)
			printf("It works\n");
			//handle_new_arch(file, file->content + fat_arch[i].offset));
	}
}

void						handle_new_arch(t_file *file, uint32_t offset)
{
	uint32_t				magic;
	t_arch					*new_arch;

	magic = *(uint32_t*)(file->content + offset);
	new_arch = (t_arch*)ft_memalloc(sizeof(t_arch));
	ft_bzero(new_arch, sizeof(t_arch));
	new_arch->offset = offset;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
	{
		if (file->is_little_endian)
			swap_32_header(file, offset);
		new_arch->name_int = ARCH_32;
		handle_32_header(file, offset, new_arch);
	}
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
	{
		;
		/*if (file->is_little_endian)
			swap_64_header(file, offset);
		new_arch->name_int = ARCH_64;
		handle_64_header(file, offset, new_arch);*/
	}
	add_arch_to_list(file, new_arch);
}

void						handle_32_header(t_file *file, uint32_t offset, t_arch *arch)
{
	struct load_command		*lc;
	struct section			*sect;
	uint32_t				ncmds;
	uint32_t				i;

	i = -1;
	ncmds = ((struct mach_header*)file->content)->ncmds;
	lc = file->content + offset + sizeof(struct mach_header);
	if (file->is_little_endian)
		swap_load_command(file, sizeof(struct mach_header));
	sect = NULL;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			if (file->is_little_endian)
				swap_32_segment_command(file, (uint32_t)((void*)lc - file->content));
			parse_32_segments(file, (struct segment_command*)lc, arch);
		}
		else if (lc->cmd == LC_SYMTAB)
		{
			if (file->is_little_endian)
				swap_symtab_command(file, (uint32_t)((void*)lc - file->content));
			parse_symtable_32(file, (struct symtab_command*)lc, arch);
		}
		lc = (void*)lc + lc->cmdsize;
	}
}
