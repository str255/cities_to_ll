/* Copyright 2017 Nicholas C. Strauss (strauss@positive-internet.com)
   This program is distributed under the terms of the GNU General Public License.
   See File COPYING for full details. 
*/
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

struct cityLocation { char *name; float latitude; float longitude; char *country; char *state; };
#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len);
#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
struct cityLocation *
in_word_set (register const char *str, register unsigned int len, 
	     char *search_state, char *search_country,
	     char *output_format, char *grouper);
void to_upper_word(char *words);
void to_allupper_word(char *words);
int parse_options (int argc, char **argv);

char    
  *search_term = NULL,
  *search_country = NULL, 
  *search_state = NULL;

int
main(int argc, char *argv[])
{
  int    nsearch;
  char  *grouper = "{}";
  char  *output_format = "%s, %s, %s, %f, %f\n";     /* name, state, country, lat, lon*/

  if (argc > 1){
    parse_options(argc, argv);
  }
  else exit(0);

  nsearch = strlen(search_term);
  to_upper_word(search_term);

  printf("search_term |%s| %d\n", search_term, nsearch);
  
  struct cityLocation *loc = in_word_set (search_term, nsearch,
					  search_state, search_country,
					  output_format, grouper);
  /*
  if (loc) 
    printf("%s %s %f %f\n", loc->name, loc->state, loc->latitude, loc->longitude);
  */
}

void
to_upper_word(char *words)
{
  char    *wp = words;
  *wp  = toupper(*wp);
  while (*wp){
    if (*(wp-1)==32) *wp = toupper(*wp);
    wp++;
  }
}

void
to_allupper_word(char *words)
{
  char    *wp = words;
  *wp  = toupper(*wp);
  while (*wp){
    *wp = toupper(*wp);
    wp++;
  }
}


int
parse_options (int argc, char **argv)
{

  int c;

  while (1)
    {
            static struct option long_options[] =
	      {
		{"term", required_argument, 0, 't'},
		{"state", required_argument, 0, 's'},
		{"country", required_argument, 0, 'c'},
		{"help",    no_argument,       0, 'h'},
		{0, 0, 0, 0}
	      };
	    /* getopt_long stores the option index here. */
	    int option_index = 0;

	    c = getopt_long (argc, argv, "t:s:c:h",
			     long_options, &option_index);

	    /* Detect the end of the options. */
	    if (c == -1)
	      break;

	    switch (c)
	      {

	      case 't':
		printf ("option -t with value `%s'\n", optarg);
		search_term = strdup(optarg);
		break;
		
	      case 's':
		printf ("option -s with value `%s'\n", optarg);
		search_state = strdup(optarg);
		to_allupper_word(search_state);
		break;

	      case 'c':
		printf ("option -c with value `%s'\n", optarg);
		search_country = strdup(optarg);
		to_allupper_word(search_country);
		break;

	      case 'h':
		printf ("city_to_ll --term --state --country --help\n");
		printf ("\t--term <search-term>\n");
		printf ("\t--state <search-state>\n");
		printf ("\t--country <search-country>\n");				
		break;

	      default:
		exit(0);
	      }
    }


  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
	printf ("%s ", argv[optind++]);
      putchar ('\n');
    }

  return 0;
}
