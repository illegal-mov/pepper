#include <string>
#include "PeFile.h"

using namespace Pepper;

PeFile::PeFile(std::string path)
: m_fname (path)
, m_Bytes (path)
, m_Dos   (m_Bytes)
, m_File  (m_Bytes, m_Dos)
, m_Opt   (m_Bytes, m_File)
, m_DDir  (m_Bytes, m_Opt)
, m_Sctns (m_Bytes, m_File)
, m_Xprt  (*this, m_Bytes, m_DDir.directories()[DataDirectory::XPRT])
, m_Mprt  (*this, m_Bytes, m_DDir.directories()[DataDirectory::MPRT])
, m_Rsrc  (*this, m_Bytes, m_DDir.directories()[DataDirectory::RSRC])
, m_Xcpt  (*this, m_Bytes, m_DDir.directories()[DataDirectory::XCPT])
, m_Cert  (*this, m_Bytes, m_DDir.directories()[DataDirectory::CERT])
, m_Reloc (*this, m_Bytes, m_DDir.directories()[DataDirectory::RLOC])
, m_Dbg   (*this, m_Bytes, m_DDir.directories()[DataDirectory::DBG])
, m_Rchtr (*this, m_Bytes, m_DDir.directories()[DataDirectory::ARCH])
, m_Gptr  (*this, m_Bytes, m_DDir.directories()[DataDirectory::GLBPTR])
, m_Tls   (*this, m_Bytes, m_DDir.directories()[DataDirectory::TLS])
, m_Ldcfg (*this, m_Bytes, m_DDir.directories()[DataDirectory::LDCFG])
, m_Bmprt (*this, m_Bytes, m_DDir.directories()[DataDirectory::BMPRT])
, m_Iat   (*this, m_Bytes, m_DDir.directories()[DataDirectory::IAT])
, m_Dmprt (*this, m_Bytes, m_DDir.directories()[DataDirectory::DMPRT])
, m_Clr   (*this, m_Bytes, m_DDir.directories()[DataDirectory::CLR])
{}

const void* PeFile::getHeaderPtr(const int index) const
{
    switch (index) {
        case DOS           : return &m_Dos;
        case FILE          : return &m_File;
        case OPTIONAL      : return &m_Opt;
        case DATA_DIR      : return &m_DDir;
        case SECTION       : return &m_Sctns;
        case EXPORT        : return &m_Xprt;
        case IMPORT        : return &m_Mprt;
        case RESOURCE      : return &m_Rsrc;
        case EXCEPTION     : return &m_Xcpt;
        case CERTIFICATE   : return &m_Cert;
        case RELOCATION    : return &m_Reloc;
        case DEBUG         : return &m_Dbg ;
        case ARCHITECTURE  : return &m_Rchtr; // not used
        case GLOBAL_POINTER: return &m_Gptr;
        case TLS           : return &m_Tls;
        case LOAD_CONFIG   : return &m_Ldcfg;
        case BOUND_IMPORT  : return &m_Bmprt;
        case IAT           : return &m_Iat;
        case DELAY_IMPORT  : return &m_Dmprt;
        case CLR           : return &m_Clr;
        default            : return nullptr;
    }
}

const char* PeFile::getHeaderName(const int index)
{
    switch (index) {
        case DOS           : return "DOS Header";
        case FILE          : return "File Header";
        case OPTIONAL      : return "Optional Header";
        case DATA_DIR      : return "Data Directory";
        case SECTION       : return "Section Header";
        case EXPORT        : return "Export Directory";
        case IMPORT        : return "Import Directory";
        case RESOURCE      : return "Resource Directory";
        case EXCEPTION     : return "Exception Directory";
        case CERTIFICATE   : return "Certificate Directory";
        case RELOCATION    : return "Relocation Directory";
        case DEBUG         : return "Debug Directory";
        case ARCHITECTURE  : return "Architecture Directory";
        case GLOBAL_POINTER: return "Global Pointer Directory";
        case TLS           : return "TLS Directory";
        case LOAD_CONFIG   : return "Load Config Directory";
        case BOUND_IMPORT  : return "Bound Import Directory";
        case IAT           : return "IAT Directory";
        case DELAY_IMPORT  : return "Delay Import Directory";
        case CLR           : return "CLR Directory";
        default            : return "<UNKNOWN>";
    }
}

void PeFile::readBytes(const size_t pos, char *buf, const size_t bufLen) const
{
    m_Bytes.readBytes(pos, buf, bufLen);
}

