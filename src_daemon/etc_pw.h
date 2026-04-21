/* PASSWD(5)               File Formats and Configuration               PASSWD(5)

NAME
       passwd - the password file

DESCRIPTION
       /etc/passwd contains one line for each user account, with seven fields
       delimited by colons (“:”). These fields are:

       •   login name

       •   optional encrypted password

       •   numerical user ID

       •   numerical group ID

       •   user name or comment field

       •   user home directory

       •   optional user command interpreter

       If the password field is a lower-case “x”, then the encrypted password
       is actually stored in the shadow(5) file instead; there must be a
       corresponding line in the /etc/shadow file, or else the user account is
       invalid.
 */
