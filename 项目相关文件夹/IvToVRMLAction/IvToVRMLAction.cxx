//*********************************************************************
//
// Program ivToVRMLAction -- converts full Inventor to VRML1 or VRML2
//
// Convert from ASCII/binary Inventor
//           to VRML ASCII or  
//           to VRML2 utf8
//
// Uses the SoToVRMLAction 
//
//*********************************************************************
// uncomment the line below to keep the LightModel node in the VRML 1.0 output.
//#define KEEP_LIGHT_MODEL
#include <stdio.h>
#ifdef _WIN32
#  include "lgetopt.h"
#  define strdup _strdup
#elif defined(__hpux) || defined(_AIX) || defined(sun) || defined(__linux__) || defined(__osf__) || defined(__NUTC__)
#  include <unistd.h>
#endif
#include <string.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/actions/SoToVRMLAction.h>
#include <Inventor/actions/SoToVRML2Action.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodekits/SoBaseKit.h>

int doVRML2 = 0;
SoToVRMLAction *toVRML;
SoToVRML2Action *toVRML2;

static void
print_usage(const char *progname)
{
  fprintf(stderr, "Usage: %s [-vfntuhpqr] [-o filename] [-d InlineDir] file ...\n", progname);
  fprintf(stderr, "-v : Generate VRML2 output\n");
  fprintf(stderr, "-o : Write output to [filename]\n");
  fprintf(stderr, "-d : A directory path. When converting SoFile to Inline nodes,\n");
  fprintf(stderr, "     write converted filed to [InlineDir]\n");
  fprintf(stderr, "-f : Expand File nodes instead of converting to Inline nodes\n");
  fprintf(stderr, "-n : Force Texture Coordinates to write for all Nurbs Surfaces\n");
  fprintf(stderr, "-p : Do not reuse existing appearance nodes\n");
  fprintf(stderr, "-q : Do not reuse existing geometry property nodes\n");
  fprintf(stderr, "-r : Do not reuse existing geometry nodes\n");
  fprintf(stderr, "-t : Expand Texture2 nodes\n");
  fprintf(stderr, "-u : Write unknown nodes to output instead of removing them\n");
  fprintf(stderr, "-h : This message (help)\n");
#if defined(_WIN32)
  fprintf(stderr, "If given a filename of '-', standard input will be read.\n\n");
#else
  fprintf(stderr, "If given a filename of '-' or if not given any filenames, standard\n");
  fprintf(stderr, "input will be read.\n");
#endif
  fprintf(stderr, "If the -o option is not used to specify the output filename, the\n");
  fprintf(stderr, "converted file is written to standard output\n");
  exit(99);
}

static void
parse_args(int argc, char **argv, char **outfilename)
{
  int err = 0;        // Flag: error in options?
  int c;

  while ((c = getopt(argc, argv, "vfpqrntuo:d:h")) != -1) {
    switch(c) {
    case 'f':
      toVRML->expandSoFile(TRUE);
      toVRML2->expandSoFile(TRUE);
      break;
    case 'n':
      toVRML->writeTexCoords(TRUE);
      toVRML2->writeTexCoords(TRUE);
      break;
    case 'v':
      doVRML2 = 1;
      break;
    case 'p':
      toVRML2->reuseAppearanceNodes(FALSE);
      break;
    case 'q':
      toVRML2->reusePropertyNodes(FALSE);
      break;
    case 'r':
      toVRML2->reuseGeometryNodes(FALSE);
      break;
    case 't':
      toVRML->expandTexture2Node(TRUE);
      toVRML2->expandTexture2Node(TRUE);
      break;
    case 'u':
      toVRML->keepUnknownNodes(TRUE);
      toVRML2->keepUnknownNodes(TRUE);
      break;
    case 'd':
      toVRML->setUrlName(optarg);
      toVRML2->setUrlName(optarg);
      break;
    case 'o':
      *outfilename = optarg;
      break;
    case 'h':        // Help
    default:
      err = 1;
      break;
    }
  }

  if (err)
    print_usage(argv[0]);
}

static void
readInputStuff(int argc, char **argv, SoNodeList &nodeList)
{
  SoInput in;
  SoNode *root;

#ifndef _WIN32
  // For WINxx do *not* read stdin automatically, require use of "-"
  if (optind == argc) {
    ++argc;                    // Act like one argument "-" was given
    argv[optind] = (char *)"-";
  }
#endif
  for (; optind < argc; optind++) {
    char *filename = argv[optind];

    if (strcmp(filename, "-") == 0) {

#ifndef _WIN32
      if (isatty(fileno(stdin))) {
        fprintf(stderr, "Must either specify a file or use stdin\n");
        print_usage(argv[0]);
      }
#endif
      in.setFilePointer(stdin);
      filename = NULL;        // Tested later...
    }
    else if (in.openFile(filename) == FALSE) {
      fprintf(stderr, "Could not open file %s\n", filename);
    }

    // If the filename includes a directory path, add the directory name
    // to the list of directories where to look for input files 
    if (filename != NULL) {
      const char *slashPtr;
      char *searchPath = NULL;

      if ((slashPtr = strrchr(filename, '/')) != NULL) {
        searchPath = strdup(filename);
        searchPath[slashPtr - filename] = '\0';
        in.addDirectoryFirst(searchPath);
      }
    }

    do {
      int read_ok = SoDB::read(&in, root);
      if (!read_ok) {
        fprintf(stderr, "Error reading %s\n",
                filename == NULL ? "stdin" : filename);
        print_usage(argv[0]);
      }
      else if (root != NULL) {
        nodeList.append(root);
      }
    } while (root != NULL);

    if (filename != NULL) 
      in.closeFile();
  }
}

static void
openOutput(SoOutput &out, int binary, char *outputfile, const char *progname)
{
  out.setBinary(binary);
  if (outputfile == NULL) {
    out.setFilePointer(stdout);
  }
  else {
    if (out.openFile(outputfile) == FALSE) {
      fprintf(stderr, "Couldn't open %s for writing\n", outputfile);
      print_usage(progname);
    }
  }
}

static void
closeOutput(SoOutput &out, char *outputfile)
{
  if (outputfile != NULL) 
    out.closeFile();
}


int
main(int argc, char **argv)
{
  // Initialize Inventor
  SoInteraction::init();
  
  // Initialize our variables
  char *outputfile = NULL;

  // Create a converter:
  toVRML  = new SoToVRMLAction;
  toVRML2 = new SoToVRML2Action;
    
  // Set up defaults to match old ivtovrml program:
  toVRML2->reuseAppearanceNodes(TRUE); 
  toVRML2->reusePropertyNodes(TRUE);
  toVRML2->reuseGeometryNodes(TRUE);

  // Parse arguments
  parse_args(argc, argv, &outputfile);
  
  // read stuff from files. Roots from the files will be in nodeList:
  SoNodeList nodeList;
  readInputStuff(argc, argv, nodeList);
  
  // Move the stuff in nodeList under "umbrellaRoot." If only one node in
  // list, then use that node, otherwise create a new "umbrellaRoot."
  SoSeparator *umbrellaRoot = NULL;
  if (nodeList.getLength() == 0) {
    print_usage(argv[0]);
    exit(0);
  }
  if (nodeList.getLength() == 1) {
    if (nodeList[0]->getTypeId() == SoSeparator::getClassTypeId()) {
      umbrellaRoot = (SoSeparator *) nodeList[0];
      umbrellaRoot->ref();
    }
  }
  if (umbrellaRoot == NULL) {
    umbrellaRoot = new SoSeparator;
    umbrellaRoot->ref();
    for (int i = 0; i < nodeList.getLength(); i++)
      umbrellaRoot->addChild(nodeList[i]);
  }
  nodeList.truncate(0);
  
  SoNode *convertedRoot;

  // Perform the conversion
  if (doVRML2) {
    toVRML2->apply(umbrellaRoot);
    convertedRoot = (SoNode *) toVRML2->getVRML2SceneGraph();
  } 
  else {
    toVRML->apply(umbrellaRoot);
    convertedRoot = toVRML->getVRMLSceneGraph();
  } 
  convertedRoot->ref();

  // write stuff
  SoOutput out;

  // TGS BugFix -- mmh 8/27/96
  // Make sure synthesized DEF names are legal VRML!
  // (Default is "+0", "+1", etc but the "+" char is illegal.)
  SoBase::setInstancePrefix("_");
  
  // Make sure the header we put out is a VRML header.
  SbString  hstr;
  if (!doVRML2)
    hstr = "#VRML V1.0 ascii";
  else
    hstr = "#VRML V2.0 utf8";
  out.setHeaderString(hstr);
  openOutput(out, FALSE , outputfile, argv[0]);
  SoWriteAction writer(&out);
  writer.apply(convertedRoot);
      
  closeOutput(out, outputfile);
  
  // Free up the memory
  umbrellaRoot->unref();
  convertedRoot->unref();

  return 0;
}
#ifdef KEEP_LIGHT_MODEL
// override SoLightModel constructor function so it writes as an unknown node
#include <Inventor/nodes/SoLightModel.h>

SoLightModel::SoLightModel()
{
  SO_NODE_CONSTRUCTOR(SoLightModel);
  SO_NODE_ADD_FIELD(model, (SoLazyElement::getDefaultLightModel()));

  // Set up static info for enumerated type field
  SO_NODE_DEFINE_ENUM_VALUE(Model, BASE_COLOR);
  SO_NODE_DEFINE_ENUM_VALUE(Model, PHONG);

  // Set up info in enumerated type field
  SO_NODE_SET_SF_ENUM_TYPE(model, Model);

  isBuiltIn = FALSE;
}
#endif

