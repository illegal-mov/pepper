#include <iostream>
#include <cstdint>
#include <cstdlib>
#include "../include/pepper.h"

/* Examples of retrieving data from a PeFile object
 */

static constexpr char fmt32[] = "%-40s0x%08x\n";
static constexpr char fmt64[] = "%-40s0x%016lx\n";

void printResourceNode(const Pepper::ResourceNode &rn, int depth);

void printFileHeader(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const FileHeader &file = pe.fileHdr();
    int i=0;
    for (; i < FileHeader::TIMESTAMP; i++)
        printf("%-40s0x%04x\n", file.getFieldName(i), *static_cast<const int16_t*>(file.getFieldPtr(i)));
    for (; i < FileHeader::SIZE_OF_OPTIONAL_HEADER; i++)
        printf("%-40s0x%08x\n", file.getFieldName(i), *static_cast<const int32_t*>(file.getFieldPtr(i)));
    for (; i < FileHeader::_NUM_FIELDS; i++)
        printf("%-40s0x%04x\n", file.getFieldName(i), *static_cast<const int16_t*>(file.getFieldPtr(i)));
}

template <typename ArchType, const char *fmt>
void printOptHdrStruct(const Pepper::OptionalHeader &oh)
{
    using namespace Pepper;
    int i=0;
    for (; i < OptionalHeader::MAJOR_LINKER_VERSION; i++)
        printf("%-40s0x%04x\n", oh.getFieldName(i), *static_cast<const int16_t*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::SIZE_OF_CODE; i++)
        printf("%-40s0x%02x\n", oh.getFieldName(i), *static_cast<const int8_t*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::BASE_OF_DATA; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *static_cast<const int32_t*>(oh.getFieldPtr(i)));

    if (oh.optional32()->Magic != OptionalHeader::BIT32)
        i++; // skip BaseOfData for non-32bit binaries

    for (; i < OptionalHeader::IMAGE_BASE; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *static_cast<const int32_t*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::SECTION_ALIGNMENT; i++)
        printf(fmt, oh.getFieldName(i), *static_cast<const ArchType*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::MAJOR_OPERATING_SYSTEM_VERSION; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *static_cast<const int32_t*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::WIN32_VERSION_VALUE; i++)
        printf("%-40s0x%04x\n", oh.getFieldName(i), *static_cast<const int16_t*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::SUBSYSTEM; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *static_cast<const int32_t*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::SIZE_OF_STACK_RESERVE; i++)
        printf("%-40s0x%04x\n", oh.getFieldName(i), *static_cast<const int16_t*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::LOADER_FLAGS; i++)
        printf(fmt, oh.getFieldName(i), *static_cast<const ArchType*>(oh.getFieldPtr(i)));
    for (; i < OptionalHeader::DATA_DIRECTORY; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *static_cast<const int32_t*>(oh.getFieldPtr(i)));
}

void printOptionalHeader(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const OptionalHeader &optional = pe.optionalHdr();
    if (Ident::is32bit(pe))
        printOptHdrStruct<int32_t, fmt32>(optional);
    else
        printOptHdrStruct<int64_t, fmt64>(optional);
}

void printDataDirectory(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const DataDirectory &dataDir = pe.dataDir();
    int i=0;
    for (const auto &datadir : dataDir.directories()) {
        printf("%02d - %-16s0x%08x | 0x%08x\n", i, DataDirectory::getDirectoryEntryName(i), datadir.rva(), datadir.size());
        i++;
    }
}

void printSectionHeaders(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const SectionHeaders &sections = pe.sectionHdrs();

    for (const auto &section : sections.sections()) {
        int j=0;
        printf("%-40s%.8s\n", section.getFieldName(j), static_cast<const char*>(section.getFieldPtr(j)));
        for (j=1; j < SectionHeaderEntry::NUMBER_OF_RELOCATIONS; j++)
            printf("%-40s0x%08x\n", section.getFieldName(j), *static_cast<const int32_t*>(section.getFieldPtr(j)));
        for (; j < SectionHeaderEntry::CHARACTERISTICS; j++)
            printf("%-40s0x%04x\n", section.getFieldName(j), *static_cast<const int16_t*>(section.getFieldPtr(j)));
        printf("%-40s0x%08x\n\n", section.getFieldName(j), *static_cast<const int32_t*>(section.getFieldPtr(j)));
    }
}

void printExport(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ExportDir &xport = pe.exportDir();
    if (Ident::dirExists(xport)) {
        int i=0;
        for (; i < ExportDir::MAJOR_VERSION; i++)
            printf("%-40s0x%08x\n", xport.getFieldName(i), *static_cast<const int32_t*>(xport.getFieldPtr(i)));
        for (; i < ExportDir::NAME_RVA; i++)
            printf("%-40s0x%04x\n", xport.getFieldName(i), *static_cast<const int16_t*>(xport.getFieldPtr(i)));
        for (; i < ExportDir::_NUM_FIELDS; i++)
            printf("%-40s0x%08x\n", xport.getFieldName(i), *static_cast<const int32_t*>(xport.getFieldPtr(i)));

        std::cout << '\n' << xport.dllName() << '\n';

        const ExportAddressTable *eat = xport.eat();
        const ExportNameTable    *ent = xport.ent();
        const ExportOrdinalTable *eot = xport.eot();

        std::cout << "\n RVA        | RAW        | ORD  | NAME       | STRING\n";
        std::cout << "------------+------------+------+------------+--------\n";
        for (size_t j=0; j < eat->length(); j++) {
            printf(" 0x%08x | 0x%08x | 0x%02x | 0x%08x | %s\n", eat->codeRva(j), eat->codeRaw(j), eot->ordinal(j), ent->nameRva(j), ent->funcName(j));
        }
    }
}

template <typename RetType, typename DescriptorType, RetType (DescriptorType::*ThunkGetter)() const>
void printImportThunks(const DescriptorType &id)
{
    using namespace Pepper;
    for (const auto &thunk : (id.*ThunkGetter)()) {
        const ImportName *pin = thunk.importname();
        if (pin != nullptr)
            std::cout << "    " << static_cast<const char*>(pin->getFieldPtr(ImportName::NAME)) << '\n';
        else
            std::cout << "    " << *thunk.ordstr() << '\n';
    }
}

template <typename DirType, typename DescriptorType, const DirType& (Pepper::PeFile::*HeaderGetter)() const>
void printImportDescriptors(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const DirType &mport = (pe.*HeaderGetter)();
    if (Ident::dirExists(mport)) {
        for (const auto &descriptor : mport.descriptors()) {
            std::cout << "DLL Name: " << descriptor.dllName() << '\n';
            for (int i=0; i < DescriptorType::_NUM_FIELDS; i++) {
                printf("    %-40s0x%08x\n", descriptor.getFieldName(i), *static_cast<const int32_t*>(descriptor.getFieldPtr(i)));
            }
            printf("\n");

            if (Ident::is32bit(pe))
                printImportThunks<
                    decltype(descriptor.thunks32()),
                    DescriptorType,
                    &DescriptorType::thunks32>(descriptor);
            else
                printImportThunks<
                    decltype(descriptor.thunks64()),
                    DescriptorType,
                    &DescriptorType::thunks64>(descriptor);

            printf("\n");
        }
    }
}

void printImport(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    printImportDescriptors<ImportDir, ImportDescriptor, &PeFile::importDir>(pe);
}

void printResourceString(const Pepper::ResourceStringU &rs)
{
    using namespace Pepper;
    printf(" (%s)", rs.name().c_str());
}

void printResourceData(const Pepper::ResourceData &rd, int depth)
{
    using namespace Pepper;
    for (int i=0; i < ResourceData::_NUM_FIELDS; i++)
        printf("%*.s%-40s 0x%08x\n", depth << 2, " ", rd.getFieldName(i),
            *static_cast<const int32_t*>(rd.getFieldPtr(i)));
    printf("\n");
}

void printResourceEntry(const Pepper::ResourceEntry &re, int depth)
{
    using namespace Pepper;

    printf("%*.s%-40s 0x%08x", depth << 2, " ", re.getFieldName(0),
        *static_cast<const int32_t*>(re.getFieldPtr(0)));

    if (re.hasName()) {
        printResourceString(*re.name());
    }

    for (int i=1; i < ResourceEntry::_NUM_FIELDS; i++)
        printf("\n%*.s%-40s 0x%08x\n", depth << 2, " ", re.getFieldName(i),
            *static_cast<const int32_t*>(re.getFieldPtr(i)));
    printf("\n");

    if (re.isDirectory()) {
        printResourceNode(*re.node(), depth);
    } else {
        printResourceData(*re.data(), depth);
    }
}

void printResourceNode(const Pepper::ResourceNode &rn, int depth)
{
    using namespace Pepper;
    int i=0;
    for (; i < ResourceNode::MAJOR_VERSION; i++)
        printf("%*.s%-40s 0x%08x\n", depth << 2, " ", rn.getFieldName(i),
            *static_cast<const int32_t*>(rn.getFieldPtr(i)));
    for (; i < ResourceNode::_NUM_FIELDS; i++)
        printf("%*.s%-40s 0x%04x\n", depth << 2, " ", rn.getFieldName(i),
            *static_cast<const int16_t*>(rn.getFieldPtr(i)));

    for (const auto &entry : rn.entries())
        printResourceEntry(entry, depth+1);
}

void printResource(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ResourceDir &resource = pe.resourceDir();
    if (Ident::dirExists(resource)) {
        printResourceNode(*resource.resources(), 0);
    }
}

template <typename RetType, int _NUM_FIELDS, const RetType& (Pepper::ExceptionDir::*HeaderGetter)() const>
void printExceptionTable(const Pepper::ExceptionDir &exception)
{
    for (const auto &entry : (exception.*HeaderGetter)()) {
        for (int i=0; i < _NUM_FIELDS; i++) {
            printf("%-40s0x%08x\n", entry.getFieldName(i), *static_cast<const int32_t*>(entry.getFieldPtr(i)));
        }
        printf("\n");
    }
}

void printException(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ExceptionDir &exception = pe.exceptionDir();
    if (Ident::dirExists(exception)) {
        if (Ident::is32bit(pe))
            printExceptionTable<decltype(exception.table32()),
                FunctionTableEntry32::_NUM_FIELDS,
                &ExceptionDir::table32>(exception);
        else if (Ident::is64bit(pe))
            printExceptionTable<decltype(exception.table64()),
                FunctionTableEntry64::_NUM_FIELDS,
                &ExceptionDir::table64>(exception);
        else
            printExceptionTable<decltype(exception.tableArm()),
                FunctionTableEntryArm::_NUM_FIELDS,
                &ExceptionDir::tableArm>(exception);
    }
}

void printCertificate(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const CertificateDir &certificate = pe.certificateDir();
    if (Ident::dirExists(certificate)) {
        for (const auto &cert : certificate.certs()) {
            int i=0;
            int32_t size = *static_cast<const int32_t*>(cert.getFieldPtr(i));
            printf("%-40s0x%08x\n", cert.getFieldName(i), size); i++;
            printf("%-40s0x%04x\n", cert.getFieldName(i), *static_cast<const int16_t*>(cert.getFieldPtr(i))); i++;
            printf("%-40s0x%04x\n", cert.getFieldName(i), *static_cast<const int16_t*>(cert.getFieldPtr(i))); i++;

            /*
            int8_t *bytes = static_cast<const int8_t*>(cert.getFieldPtr(CertificateEntry::Fields::BYTES));
            for (int i=0; i < size; i++) {
                printf("%02x ", bytes[i] & 0xFF);
            }
            //*/
            printf("\n");
        }
    }
}

void printRelocation(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const RelocationDir &relocation = pe.relocationDir();
    if (Ident::dirExists(relocation)) {
        for (const auto &block : relocation.blocks()) {
            const RelocationBase *prb = block.relocBase();
            printf("0x%08x | 0x%08x\n",
                *static_cast<const int32_t*>(prb->getFieldPtr(RelocationBase::PAGE_RVA)),
                *static_cast<const int32_t*>(prb->getFieldPtr(RelocationBase::BLOCK_SIZE)));
            const RelocationTable *prt = block.relocTable();
            for (size_t i=0; i < prt->length(); i++) {
                printf("\t0x%1x | 0x%03x\n", prt->type(i), prt->offset(i));
            }
        }
    }
}

void printDebug(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const DebugDir &debug = pe.debugDir();
    if (Ident::dirExists(debug)) {
        for (const auto &entry : debug.entries()) {
            int i=0;
            for (; i < DebugEntry::MAJOR_VERSION; i++)
                printf("%-40s0x%08x\n", entry.getFieldName(i), *static_cast<const int32_t*>(entry.getFieldPtr(i)));
            for (; i < DebugEntry::TYPE; i++)
                printf("%-40s0x%04x\n", entry.getFieldName(i), *static_cast<const int16_t*>(entry.getFieldPtr(i)));
            for (; i < DebugEntry::_NUM_FIELDS; i++)
                printf("%-40s0x%08x\n", entry.getFieldName(i), *static_cast<const int32_t*>(entry.getFieldPtr(i)));

            // print rsds structure
            const DebugRsds *rsds = entry.rsds();
            i=0;
            printf("\t%-40s%.4s\n", rsds->getFieldName(i),  static_cast<const char*>(rsds->getFieldPtr(i)));

            printf("\t%-40s", rsds->getFieldName(++i));
            const char *guid = static_cast<const char*>(rsds->getFieldPtr(i));
            for (size_t j=0; j < sizeof((static_cast<PRSDSI>(nullptr))->Guid); j++)
                printf("%02x ", guid[j] & 0xFF);

            i++;
            printf("\n\t%-40s0x%08x\n", rsds->getFieldName(i), *static_cast<const int32_t*>(rsds->getFieldPtr(i)));
            i++;
            printf("\t%-40s%s\n\n", rsds->getFieldName(i),  static_cast<const char*>(rsds->getFieldPtr(i)));
        }
    }
}

void printArchitecture(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ArchitectureDir &architecture = pe.architectureDir();
    if (Ident::dirExists(architecture)) {
        printf("[DEBUG] '%s' has %s (RAW = 0x%08lx, diff = 0x%08lx)\n", pe.path().c_str(), pe.getHeaderName(PeFile::ARCHITECTURE), architecture.dirOffset(), architecture.hdrOffset() - architecture.dirOffset());
    }
}

void printGlobalPointer(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const GlobalPointerDir &globalPointer = pe.globalPointerDir();
    if (Ident::dirExists(globalPointer)) {
        printf("[DEBUG] '%s' has %s (RAW = 0x%08lx, diff = 0x%08lx)\n", pe.path().c_str(), pe.getHeaderName(PeFile::GLOBAL_POINTER), globalPointer.dirOffset(), globalPointer.hdrOffset() - globalPointer.dirOffset());
    }
}

template <typename ArchType, const char *fmt, const Pepper::CallbacksTable<ArchType>* (Pepper::TlsDir::*HeaderGetter)() const>
void printTlsStruct(const Pepper::TlsDir& tls)
{
    using namespace Pepper;
    int i=0;
    for (; i < TlsDir::SIZE_OF_ZERO_FILL; i++)
        printf(fmt, tls.getFieldName(i), *static_cast<const ArchType*>(tls.getFieldPtr(i)));
    for (; i < TlsDir::_NUM_FIELDS; i++)
        printf("%-40s0x%08x\n", tls.getFieldName(i), *static_cast<const int32_t*>(tls.getFieldPtr(i)));

    printf("Callbacks Table\n");
    const CallbacksTable<ArchType> *pct = (tls.*HeaderGetter)();
    for (size_t i=0; i < pct->length(); i++) {
        printf(fmt, "", pct->callbackRaw(i));
    }
}

void printTls(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const TlsDir &tls = pe.tlsDir();
    if (Ident::dirExists(tls)) {
        if (Ident::is32bit(pe))
            printTlsStruct<uint32_t, fmt32, &TlsDir::cbt32>(tls);
        else
            printTlsStruct<uint64_t, fmt64, &TlsDir::cbt64>(tls);
    }
}

template <typename ArchType, const char *fmt>
void printLdCfgStruct(const Pepper::LoadConfigDir &ldcfg)
{
    using namespace Pepper;
    int i=0;
    const void *arg = nullptr;
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::MAJOR_VERSION)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *static_cast<const int32_t*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::GLOBAL_FLAGS_CLEAR)
        printf("%-40s0x%04x\n", ldcfg.getFieldName(i++), *static_cast<const int16_t*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::DECOMMIT_FREE_BLOCK_THRESHOLD)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *static_cast<const int32_t*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::PROCESS_HEAP_FLAGS)
        printf(fmt, ldcfg.getFieldName(i++), *static_cast<const ArchType*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::CSD_VERSION)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *static_cast<const int32_t*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::EDIT_LIST)
        printf("%-40s0x%04x\n", ldcfg.getFieldName(i++), *static_cast<const int16_t*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::GUARD_FLAGS)
        printf(fmt, ldcfg.getFieldName(i++), *static_cast<const ArchType*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::CODE_INTEGRITY)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *static_cast<const int32_t*>(arg));

    // CodeIntegrity
    if ((arg = ldcfg.getFieldPtr(i)) != nullptr) {
        printf("%-40s :\n", ldcfg.getFieldName(i++));
        int j=0;
        const CodeIntegrity *pci = static_cast<const CodeIntegrity*>(arg);
        for (; j < CodeIntegrity::CATALOG_OFFSET; j++)
            printf("  %-40s0x%04x\n", pci->getFieldName(j), *static_cast<const int16_t*>(pci->getFieldPtr(j)));
        for (; j < CodeIntegrity::_NUM_FIELDS; j++)
            printf("  %-40s0x%08x\n", pci->getFieldName(j), *static_cast<const int32_t*>(pci->getFieldPtr(j)));
    }

    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::DYNAMIC_VALUE_RELOC_TABLE_OFFSET)
        printf(fmt, ldcfg.getFieldName(i++), *static_cast<const ArchType*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::DYNAMIC_VALUE_RELOC_TABLE_SECTION)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *static_cast<const int32_t*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::GUARD_RF_VERIFY_STACK_POINTER_FUNCTION_POINTER)
        printf("%-40s0x%04x\n", ldcfg.getFieldName(i++), *static_cast<const int16_t*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::HOT_PATCH_TABLE_OFFSET)
        printf(fmt, ldcfg.getFieldName(i++), *static_cast<const ArchType*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::ENCLAVE_CONFIG_POINTER)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *static_cast<const int32_t*>(arg));
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::_NUM_FIELDS)
        printf(fmt, ldcfg.getFieldName(i++), *static_cast<const ArchType*>(arg));
}

void printLoadConfig(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const LoadConfigDir &loadConfig = pe.loadConfigDir();
    if (Ident::dirExists(loadConfig)) {
        if (Ident::is32bit(pe))
            printLdCfgStruct<int32_t, fmt32>(loadConfig);
        else
            printLdCfgStruct<int64_t, fmt64>(loadConfig);
    }
}

void printBoundImport(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const BoundImportDir &boundImport = pe.boundImportDir();
    if (Ident::dirExists(boundImport)) {
        printf("[DEBUG] '%s' has %s (RAW = 0x%08lx, diff = 0x%08lx)\n", pe.path().c_str(), pe.getHeaderName(PeFile::BOUND_IMPORT), boundImport.dirOffset(), boundImport.hdrOffset() - boundImport.dirOffset());
    }
}

template <typename ArchType, ArchType (Pepper::IatDir::*HeaderGetter)() const>
void printAddressesList(const Pepper::IatDir &id)
{
    using namespace Pepper;
    for (const auto &list : (id.*HeaderGetter)()) {
        for (size_t i=0; i < list.length(); i++) {
            std::cout << std::hex << "0x" << list.address(i) << '\n';
        }
        printf("\n");
    }
}

void printIat(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const IatDir &iat = pe.iatDir();
    if (Ident::dirExists(iat)) {
        if (Ident::is32bit(pe))
            printAddressesList<
                decltype(iat.list32()),
                &IatDir::list32>(iat);
        else
            printAddressesList<
                decltype(iat.list64()),
                &IatDir::list64>(iat);
    }
}

void printDelayImport(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    printImportDescriptors<DelayImportDir, DelayImportDescriptor, &PeFile::delayImportDir>(pe);
}

void printClrMetadata(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrMetadata &metadata = clr.metadataHdr();
    if (Ident::dirExists(metadata)) {
        std::cout << "\nCLR Metadata\n";
        printf("%-40s%.4s\n", metadata.getFieldName(0), static_cast<const char*>(metadata.getFieldPtr(0)));
        int i=1;
        for (; i <= ClrMetadata::MINOR_VERSION; i++) {
            printf("%-40s0x%04x\n", metadata.getFieldName(i), *static_cast<const int16_t*>(metadata.getFieldPtr(i)));
        }

        for (; i <= ClrMetadata::LENGTH; i++) {
            printf("%-40s0x%04x\n", metadata.getFieldName(i), *static_cast<const int32_t*>(metadata.getFieldPtr(i)));
        }

        printf("%-40s%s\n", metadata.getFieldName(i), static_cast<const char*>(metadata.getFieldPtr(i)));

        for (i++; i < ClrMetadata::_NUM_FIELDS; i++) {
            printf("%-40s0x%04x\n", metadata.getFieldName(i), *static_cast<const int16_t*>(metadata.getFieldPtr(i)));
        }

        printf("\n");
        for (const auto &stream : metadata.streams()) {
            printf("0x%08x 0x%08x %s\n",
                *static_cast<const int32_t*>(stream.getFieldPtr(ClrStream::OFFSET)),
                *static_cast<const int32_t*>(stream.getFieldPtr(ClrStream::SIZE)),
                 static_cast<const char   *>(stream.getFieldPtr(ClrStream::NAME)));
        }
    }
}

void printClrResource(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrResource &resource = clr.resourceHdr();
    if (Ident::dirExists(resource)) {
        std::cout << "CLR Resource\n";
    }
}

void printClrSignature(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrSignature &signature = clr.signatureHdr();
    if (Ident::dirExists(signature)) {
        std::cout << "CLR Signature\n";
        for (size_t i=0; i < signature.size(); i++) {
            printf("%02x ", signature.sig()[i] & 0xFF);
        }
        printf("\n");
    }
}

void printClrCodeManager(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrCodeManager &codeManager = clr.codeManagerHdr();
    if (Ident::dirExists(codeManager)) {
        std::cout << "CLR Code Manager\n";
    }
}

void printClrVTableFixup(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrVTableFixup &vtableFixup = clr.vTableFixupHdr();
    if (Ident::dirExists(vtableFixup)) {
        std::cout << "CLR VTable Fixups\n";
    }
}

void printClrExportJump(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrExportJump &exportJump = clr.exportJumpHdr();
    if (Ident::dirExists(exportJump)) {
        std::cout << "CLR Export Address Table Jumps\n";
    }
}

void printClrNativeHeader(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrNativeHeader &native = clr.nativeHdr();
    if (Ident::dirExists(native)) {
        std::cout << "CLR Native Header\n";
    }
}

void printClr(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ClrDir &clr = pe.clrDir();
    if (Ident::dirExists(clr)) {
        int i=0;
        for (; i <= ClrDir::SIZE; i++)
            printf("%-40s0x%08x\n", clr.getFieldName(i), *static_cast<const int32_t*>(clr.getFieldPtr(i)));
        for (; i <= ClrDir::MINOR_RUNTIME_VERSION; i++)
            printf("%-40s0x%04x\n", clr.getFieldName(i), *static_cast<const int16_t*>(clr.getFieldPtr(i)));
        for (; i < ClrDir::_NUM_FIELDS; i++)
            printf("%-40s0x%08x\n", clr.getFieldName(i), *static_cast<const int32_t*>(clr.getFieldPtr(i)));

        printf("\n");
        i=0;
        for (const auto &dataDir : clr.directories()) {
            printf("%02d - %-32s0x%08x | 0x%08x\n", i, ClrDir::getHeaderName(i), dataDir.rva(), dataDir.size());
            i++;
        }

        printClrMetadata(clr);
        printClrResource(clr);
        printClrSignature(clr);
        printClrCodeManager(clr);
        printClrVTableFixup(clr);
        printClrExportJump(clr);
        printClrNativeHeader(clr);
    }
}

int main(int argc, char *argv[])
{
//  std::cout << "MAIN\n\n";

    try {
        std::cout << argv[argc-1] << '\n';

        Pepper::PeFile pe(argv[argc-1]);

//      printFileHeader(pe);
//      printOptionalHeader(pe);
//      printDataDirectory(pe);
//      printSectionHeaders(pe);

//      printExport(pe);
//      printImport(pe);
//      printResource(pe);
//      printException(pe);
//      printCertificate(pe);
//      printRelocation(pe);
//      printDebug(pe);
//      printArchitecture(pe);
//      printGlobalPointer(pe);
//      printTls(pe);
//      printLoadConfig(pe);
//      printBoundImport(pe);
//      printIat(pe);
//      printDelayImport(pe);
//      printClr(pe);

    } catch (const Pepper::PeException &e) {
        std::cerr << argv[argc-1] << " : " << e.getError() << '\n';
    }
    //*/

    return 0;
}

