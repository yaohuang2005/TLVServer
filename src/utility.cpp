/*  TLVServer
 *  ===============
 *  Copyright (C) 2017 yaohuang2005@gmail.com
 *
 *  Licensed under the GNU LESSER GENERAL PUBLIC LICENSE
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include <stdarg.h>      // va_list
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "utility.h"

#ifdef GEN_TRACE_FILE
FILE *_apptrace;
#endif

// Print work information
void echo(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char str[200];
    sprintf(str, "[%lu] ", pthread_self());
    strcat(str, format);
    vfprintf(stderr, str, args);
    va_end(args);
}

// Print error information
int errexit(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    echo(format, args);
    va_end(args);
    exit(1);
}
