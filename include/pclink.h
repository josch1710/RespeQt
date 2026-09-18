/*
 * pclink.h
 *
 * This file contains code from the SIO2BSD project by KMK (drac030)
 *
 * Copyright 2016 TheMontezuma
 *
 */

#ifndef PCLINK_H
#define PCLINK_H

#include "sdxprotocol.h"

class PCLINK : public SDXProtocol {
  Q_OBJECT

public:
  explicit PCLINK(const SioWorkerPtr &worker);
  ~PCLINK() override = default;
  void handleCommand(quint8 command, quint8 aux1, quint8 aux2) override;
  // links are numbered from 1 to 15
  bool hasLink(int no);
  void setLink(int no, const char *fileName);
  void swapLinks(int from, int to);
  void resetLink(int no);

private:
  void unix_time_2_sdx(const time_t *todp, uchar *ob);
  bool isSDXLegalChar(uchar c);
  long dos_2_term(uchar c);
  long validate_fn(const uchar *name, int len);
  void ugefina(char *src, char *out);
  void uexpand(const uchar *rawname, char *name83);
  int match_dos_names(const char *name, const char *mask, uchar fatr1, const struct stat *sb);
  int validate_dos_name(char *fname);
  // ReSharper disable once CppRedundantElaboratedTypeSpecifier
  int check_dos_name(char *newpath, struct dirent *dp, struct stat *sb);
  void fps_close(int i);
  off_t get_file_len(uchar handle);
  DIRENTRY *cache_dir(uchar handle);
  size_t dir_read(uchar *mem, ulong blk_size, uchar handle, int *eof_sig);
  void do_pclink_init(int force);
  void set_status_size(uchar cunit, ushort size);
  int validate_user_path(const char* defwd, const char* newpath);
  bool isSDXPathSeparator(uchar c);
  void path_copy(uchar *dst, uchar *src);
  void create_user_path(uchar cunit, char *newpath);
  time_t timestamp2mtime(const uchar *stamp);
  void do_pclink(uchar devno, uchar ccom, uchar caux1, uchar caux2);
  bool is_fname_reserved(const char *fname, size_t length = ULONG_MAX) const;
  bool is_fname_encoded(const char *fname) const;
};

#endif// PCLINK_H
