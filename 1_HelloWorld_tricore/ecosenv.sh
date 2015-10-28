
EZS_UID=$$ ; export EZS_UID
EZS_BASE=/proj/i4ezs/tools ; export EZS_BASE
#ECOS_BASE_DIR=$EZS_BASE/ecos/repo_ws14.old
ECOS_BASE_DIR=$EZS_BASE/ecos/repo_ws14
#EZS_COMPILER_DIR=/proj/i4ciao/tools/trigcc346-sijesche ; export EZS_COMPILER_DIR
EZS_COMPILER_DIR=/proj/i4ciao/tools/trigcc464 ; export EZS_COMPILER_DIR
EZS_CMAKE_MODULE_DIR=$ECOS_BASE_DIR/tools ; export EZS_CMAKE_MODULE_DIR
EZS_T32_DIR=/proj/i4ciao/tools/t32-20130614 ;export EZS_T32_DIR
T32SYS=$EZS_T32_DIR; export T32SYS
T32TMP=/tmp ; export T32TMP
ECOS_REPOSITORY=$ECOS_BASE_DIR/packages ; export ECOS_REPOSITORY

if [[ $PATH != *ecos* ]] ;	then
	# eCos paths - do not modify this line, it is used by the installer
	PATH=$ECOS_BASE_DIR/tools:$EZS_COMPILER_DIR/bin:$EZS_T32_DIR/bin/pc_linux64:$PATH ; export PATH
fi
