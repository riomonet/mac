/*
SHADOW(5)               File Formats and Configuration               SHADOW(5)

NAME
       shadow - shadowed password file

DESCRIPTION
       shadow is a file which contains the password information for the
       system's accounts and optional aging information.

       This file must not be readable by regular users if password security is
       to be maintained.

       Each line of this file contains 9 fields, separated by colons (“:”), in
       the following order:

       login name
           It must be a valid account name, which exist on the system.

       encrypted password
           This field may be empty, in which case no passwords are required to
           authenticate as the specified login name. However, some
           applications which read the /etc/shadow file may decide not to
           permit any access at all if the password field is empty.

           A password field which starts with an exclamation mark means that
           the password is locked. The remaining characters on the line
           represent the password field before the password was locked.

           Refer to crypt(3) for details on how this string is interpreted.

           If the password field contains some string that is not a valid
           result of crypt(3), for instance ! or *, the user will not be able
           to use a unix password to log in (but the user may log in the
           system by other means).

       date of last password change
           The date of the last password change, expressed as the number of
           days since Jan 1, 1970 00:00 UTC.

           The value 0 has a special meaning, which is that the user should
           change her password the next time she will log in the system.

           An empty field means that password aging features are disabled.

       minimum password age
           The minimum password age is the number of days the user will have
           to wait before she will be allowed to change her password again.

           An empty field and value 0 mean that there is no minimum password
           age.

       maximum password age
           The maximum password age is the number of days after which the user
           will have to change her password.

           After this number of days is elapsed, the password may still be
*/