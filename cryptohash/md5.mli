(*
 *	Copyright (C) 2006-2009 Vincent Hanquez <tab@snarc.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; version 2.1 or version 3.0 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *)

(** Md5 OCaml binding *)

(** context type - opaque *)
type ctx

(** digest type - opaque *)
type t = string

(** init a new Md5 context *)
val init : unit -> ctx

(** Md5.update ctx data ofs len update a context with data *)
val update : ctx -> string -> int -> int -> unit

(** Md5.finalize ctx returns a finalized digest *)
val finalize : ctx -> t

(** Return the digest of the given string. *)
val string : string -> t

(** Md5.substring s ofs len returns the digest of the substring of s starting
at character number ofs and containing len characters. *)
val substring : string -> int -> int -> t

(** If len is nonnegative, Md5.channel ic len reads len characters from
channel ic and returns their digest, or raises End_of_file if end-of-file is
reached before len characters are read. If len is negative, Md5.channel ic
len reads all characters from ic until end-of-file is reached and return their
digest. *)
val channel : in_channel -> int -> t

(** Return the digest of the file whose name is given. *)
val file : string -> t

(** Return the digest of the file whose name is given using fast C function *)
val file_fast : string -> t

(** Write a digest on the given output channel. *)
val output : out_channel -> t -> unit

(** Read a digest from the given input channel. *)
val input : in_channel -> t

(** return a printable hexadecimal representation of the given digest *)
val to_hex : t -> string
