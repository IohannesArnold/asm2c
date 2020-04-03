/* unistd2.b
 * Copyright (C) 2020 John Arnold
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

execvpe(file, argv, envp) {
    auto p, path = 0; /* TODO getenv("PATH"); */
    auto len;
    auto buf;
    auto split;

    if (!*file) return -1;

    if (strchr(file, '/'))
        return execve(file, argv, envp);

    if (!path) path = "/usr/local/bin:/bin:/usr/bin";
    p = path;
    len = strlen(file) + strlen(path) + 2;
    buf = malloc(len);

    while(*p != '\0') {
        split = strchr(p, ':');
        if (split) {
            memcpy(buf, p, split-p);
	    split++;
	    buf[split-p] = '\0';
        } else {
            strcpy(buf, p);
        }
        strcat(buf, "/");
        strcat(buf, file);
        execve(buf, argv, envp);
	p = split;
    }
    return -1;
}

