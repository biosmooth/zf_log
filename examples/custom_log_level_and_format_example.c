#if defined(_WIN32) || defined(_WIN64)
	#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <zf_log.h>
#include <zf_log.c>

//VARS
/* Flag set by ‘--help’. */
static int help_flag;
static int finalLogLevel = 3;  //by default log info logs and above
static int finalLogFormat = 0;

// DECLARATIONS
void commandline_args_parse(int argc, char **argv);
void print_usage();
static void custom_output_callback(const zf_log_message *msg, void *arg);

//MAIN
int main(int argc, char *argv[])
{
	commandline_args_parse(argc,argv);
	zf_log_set_output_level(finalLogLevel);
	zf_log_set_output_v(finalLogFormat, 0, custom_output_callback);

	for (int i = 1;i <= 10;i++) { 
	ZF_LOGI("Writing output of exemplary loop,%i run", i);
	}
	//ZF_LOGI_MEM(argv, argc * sizeof(*argv), "argv pointers:");
	ZF_LOGW("This example handles printng to stdout only\n");

	return 0;
}


//FUNCTIONS
static void custom_output_callback(const zf_log_message *msg, void *arg)
{
	(void)arg;
	*msg->p = '\n';
	fwrite(msg->buf, msg->p - msg->buf + 1, 1, stdout);
	fflush(stdout);
}



void commandline_args_parse(int argc, char **argv)
{
  int c;
  int intContext = 1;
  int intTag = 2;
  int intSource = 4;
  int intMessage = 8;

  //don't forget about opterror: http://stackoverflow.com/questions/6450152/getopt-value-stays-null
  //opterr=0;
  

  while (1)
    {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"help", no_argument,       &help_flag, 1},
          /* These options don’t set a flag.
             We distinguish them by their indices. */
          {"setLogLevel", required_argument, 0, 'l'},
          {"LogContext", no_argument,        0, 'C'},
          {"LogTag",  no_argument,        0, 'T'},
          {"LogSource",  no_argument,     0, 'S'},
          {"LogMessage",  no_argument,    0, 'M'},
          {"help",  no_argument,    0, 'h'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "l:CTSMh",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;
        
	case 'l':
          //printf ("option -l with value %s\n", optarg);
	  finalLogLevel = atoi(optarg);
          break;

        case 'C':
          //puts ("option -C\n");
	  finalLogFormat += intContext;
          break;

        case 'T':
          //puts ("option -T\n");
	  finalLogFormat += intTag;
          break;

        case 'S':
          //puts ("option -S\n");
	  finalLogFormat += intSource;
          break;

        case 'M':
          //puts ("option -M\n");
	  finalLogFormat += intMessage;
          break;
        
	case 'h':
          //puts ("option -M\n");
	  print_usage();
	  exit(-1);

        case '?':
          /* getopt_long already printed an error message. */
      	  printf("see --help for usage\n");
	  exit(-1);
	  

        default:
          abort ();
        }
    }

  /* Instead of reporting ‘--help’
     we report the final status resulting from them. */
  if (help_flag) { 
    //puts ("help flag is set");
    print_usage();
    exit(-1);
  }

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc)
        printf ("%s ", argv[optind++]);
      putchar ('\n');
    }
  if (finalLogFormat == 0) { finalLogFormat = 255;} //by default log everything
  if (finalLogLevel > 6 || finalLogLevel < 1) { 
      printf("unexpected value for -l parameter: only logLevel from 1 to 6 allowed:\n");
      printf("see --help for more details\n");
      //print_usage();
  }
  //printf("selected logLevel:%d\nselected logFormat:%d\nhelpFlag:%d\n",finalLogLevel,finalLogFormat,help_flag);

}

void print_usage() {
printf("USAGE\n");
printf("customized_logging_options_example [-CTSM] -l numb\n");
printf("\n");
printf("EXAMPLES\n");
printf("customized_logging_options_example -l 1 	(print all logs in default format)\n");
printf("customized_logging_options_example -l 3 -M 	(print only INFO logs and above with custom format: messages only)\n");
printf("customized_logging_options_example -l 5 -CM 	(print only ERROR logs and above with custom format: messages preceded by context)\n");
printf("customized_logging_options_example -CSM 	(print all logs with custom format: messages preceded by context and source)\n");
printf("\n");
printf("ABBREVIATIONS\n\
Custom log settings\n\
 C - context (date: YY-MM-DD and log level: single char, e.g. W for WARN, I for INFO)\n\
 T - tag\n\
 S - source\n\
 M - message\n\
 \n\
Log Levels\n\
 1 - VERBOSE\n\
 2 - DEBUG\n\
 3 - WARNING\n\
 4 - INFO\n\
 5 - ERROR\n\
 6 - FATAL\n\
");
printf("###############################################################\n");
}
