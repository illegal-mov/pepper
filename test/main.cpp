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
    FileHeader *pfh = (FileHeader*)pe.getHeaderPtr(PeFile::FILE);
    int i=0;
    for (; i < FileHeader::TIMESTAMP; i++)
        printf("%-40s0x%04x\n", pfh->getFieldName(i), *(int16_t*)pfh->getFieldPtr(i));
    for (; i < FileHeader::SIZE_OF_OPTIONAL_HEADER; i++)
        printf("%-40s0x%08x\n", pfh->getFieldName(i), *(int32_t*)pfh->getFieldPtr(i));
    for (; i < FileHeader::_NUM_FIELDS; i++)
        printf("%-40s0x%04x\n", pfh->getFieldName(i), *(int16_t*)pfh->getFieldPtr(i));
}

template <typename T, const char *fmt>
void printOptHdrStruct(const Pepper::OptionalHeader &oh)
{
    using namespace Pepper;
    int i=0;
    for (; i < OptionalHeader::MAJOR_LINKER_VERSION; i++)
        printf("%-40s0x%04x\n", oh.getFieldName(i), *(int16_t*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::SIZE_OF_CODE; i++)
        printf("%-40s0x%02x\n", oh.getFieldName(i), *(int8_t*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::BASE_OF_DATA; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *(int32_t*)oh.getFieldPtr(i));

    if (oh.optional32()->Magic != OptionalHeader::BIT32)
        i++; // skip BaseOfData for non-32bit binaries

    for (; i < OptionalHeader::IMAGE_BASE; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *(int32_t*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::SECTION_ALIGNMENT; i++)
        printf(fmt, oh.getFieldName(i), *(T*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::MAJOR_OPERATING_SYSTEM_VERSION; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *(int32_t*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::WIN32_VERSION_VALUE; i++)
        printf("%-40s0x%04x\n", oh.getFieldName(i), *(int16_t*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::SUBSYSTEM; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *(int32_t*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::SIZE_OF_STACK_RESERVE; i++)
        printf("%-40s0x%04x\n", oh.getFieldName(i), *(int16_t*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::LOADER_FLAGS; i++)
        printf(fmt, oh.getFieldName(i), *(T*)oh.getFieldPtr(i));
    for (; i < OptionalHeader::DATA_DIRECTORY; i++)
        printf("%-40s0x%08x\n", oh.getFieldName(i), *(int32_t*)oh.getFieldPtr(i));
}

void printOptionalHeader(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    OptionalHeader *poh = (OptionalHeader*)pe.getHeaderPtr(PeFile::OPTIONAL);
    if (Ident::is32bit(pe))
        printOptHdrStruct<int32_t, fmt32>(*poh);
    else
        printOptHdrStruct<int64_t, fmt64>(*poh);
}

void printDataDirectory(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    DataDirectory *pdd = (DataDirectory*)pe.getHeaderPtr(PeFile::DATA_DIR);
    int i=0;
    for (const auto &datadir : pdd->directories()) {
        printf("%02d - %-16s0x%08x | 0x%08x\n", i, DataDirectory::getDirectoryEntryName(i), datadir.rva(), datadir.size());
        i++;
    }
}

void printSectionHeaders(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    SectionHeaders *psh = (SectionHeaders*)pe.getHeaderPtr(PeFile::SECTION);

    for (const auto &section : psh->sections()) {
        int j=0;
        printf("%-40s%.8s\n", section.getFieldName(j), (char*)section.getFieldPtr(j));
        for (j=1; j < SectionHeaderEntry::NUMBER_OF_RELOCATIONS; j++)
            printf("%-40s0x%08x\n", section.getFieldName(j), *(int32_t*)section.getFieldPtr(j));
        for (; j < SectionHeaderEntry::CHARACTERISTICS; j++)
            printf("%-40s0x%04x\n", section.getFieldName(j), *(int16_t*)section.getFieldPtr(j));
        printf("%-40s0x%08x\n\n", section.getFieldName(j), *(int32_t*)section.getFieldPtr(j));
    }
}

void printExport(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ExportDir *ped = (ExportDir*)pe.getHeaderPtr(PeFile::EXPORT);
    if (Ident::dirExists(*ped)) {
        int i=0;
        for (; i < ExportDir::MAJOR_VERSION; i++)
            printf("%-40s0x%08x\n", ped->getFieldName(i), *(int32_t*)ped->getFieldPtr(i));
        for (; i < ExportDir::NAME_RVA; i++)
            printf("%-40s0x%04x\n", ped->getFieldName(i), *(int16_t*)ped->getFieldPtr(i));
        for (; i < ExportDir::_NUM_FIELDS; i++)
            printf("%-40s0x%08x\n", ped->getFieldName(i), *(int32_t*)ped->getFieldPtr(i));

        std::cout << '\n' << ped->dllName() << '\n';

        const ExportAddressTable *eat = ped->eat();
        const ExportNameTable    *ent = ped->ent();
        const ExportOrdinalTable *eot = ped->eot();

        std::cout << "\n RVA        | RAW        | ORD  | NAME       | STRING\n";
        std::cout << "------------+------------+------+------------+--------\n";
        for (size_t j=0; j < eat->length(); j++) {
            printf(" 0x%08x | 0x%08x | 0x%02x | 0x%08x | %s\n", eat->codeRva(j), eat->codeRaw(j), eot->ordinal(j), ent->nameRva(j), ent->funcName(j));
        }
    }
}

template <typename T, typename U, T (U::*V)() const>
void printImportThunks(const U &id)
{
    using namespace Pepper;
    for (const auto &thunk : (id.*V)()) {
        const ImportName *pin = thunk.importname();
        if (pin != nullptr)
            std::cout << "    " << (char*)pin->getFieldPtr(ImportName::NAME) << '\n';
        else
            std::cout << "    " << *thunk.ordstr() << '\n';
    }
}

template <typename T, typename U, int dirIndex>
void printImportDescriptors(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const T *pid = (T*)pe.getHeaderPtr(dirIndex);
    if (Ident::dirExists(*pid)) {
        for (const auto &descriptor : pid->descriptors()) {
            std::cout << "DLL Name: " << descriptor.dllName() << '\n';
            for (int i=0; i < U::_NUM_FIELDS; i++) {
                printf("    %-40s0x%08x\n", descriptor.getFieldName(i), *(int32_t*)descriptor.getFieldPtr(i));
            }
            printf("\n");

            if (Ident::is32bit(pe))
                printImportThunks<
                    decltype(descriptor.thunks32()),
                    U,
                    &U::thunks32>(descriptor);
            else
                printImportThunks<
                    decltype(descriptor.thunks64()),
                    U,
                    &U::thunks64>(descriptor);

            printf("\n");
        }
    }
}

void printImport(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    printImportDescriptors<ImportDir, ImportDescriptor, PeFile::IMPORT>(pe);
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
            *(int32_t*)rd.getFieldPtr(i));
    printf("\n");
}

void printResourceEntry(const Pepper::ResourceEntry &re, int depth)
{
    using namespace Pepper;

    printf("%*.s%-40s 0x%08x", depth << 2, " ", re.getFieldName(0),
        *(int32_t*)re.getFieldPtr(0));

    if (re.hasName()) {
        printResourceString(*re.name());
    }

    for (int i=1; i < ResourceEntry::_NUM_FIELDS; i++)
        printf("\n%*.s%-40s 0x%08x\n", depth << 2, " ", re.getFieldName(i),
            *(int32_t*)re.getFieldPtr(i));
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
            *(int32_t*)rn.getFieldPtr(i));
    for (; i < ResourceNode::_NUM_FIELDS; i++)
        printf("%*.s%-40s 0x%04x\n", depth << 2, " ", rn.getFieldName(i),
            *(int16_t*)rn.getFieldPtr(i));

    for (const auto &entry : rn.entries())
        printResourceEntry(entry, depth+1);
}

void printResource(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ResourceDir *prd = (ResourceDir*)pe.getHeaderPtr(PeFile::RESOURCE);
    if (Ident::dirExists(*prd)) {
        printResourceNode(*prd->resources(), 0);
    }
}

template <typename T, int _NUM_FIELDS, const T& (Pepper::ExceptionDir::*U)() const>
void printExceptionTable(const Pepper::ExceptionDir &ped)
{
    for (const auto &entry : (ped.*U)()) {
        for (int i=0; i < _NUM_FIELDS; i++) {
            printf("%-40s0x%08x\n", entry.getFieldName(i), *(int32_t*)entry.getFieldPtr(i));
        }
        printf("\n");
    }
}

void printException(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ExceptionDir *ped = (ExceptionDir*)pe.getHeaderPtr(PeFile::EXCEPTION);
    if (Ident::dirExists(*ped)) {
        if (Ident::is32bit(pe))
            printExceptionTable<decltype(ped->table32()),
                FunctionTableEntry32::_NUM_FIELDS,
                &ExceptionDir::table32>(*ped);
        else if (Ident::is64bit(pe))
            printExceptionTable<decltype(ped->table64()),
                FunctionTableEntry64::_NUM_FIELDS,
                &ExceptionDir::table64>(*ped);
        else
            printExceptionTable<decltype(ped->tableArm()),
                FunctionTableEntryArm::_NUM_FIELDS,
                &ExceptionDir::tableArm>(*ped);
    }
}

void printCertificate(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const CertificateDir *pcd = (CertificateDir*)pe.getHeaderPtr(PeFile::CERTIFICATE);
    if (Ident::dirExists(*pcd)) {
        for (const auto &cert : pcd->certs()) {
            int i=0;
            int32_t size = *(int32_t*)cert.getFieldPtr(i);
            printf("%-40s0x%08x\n", cert.getFieldName(i), size); i++;
            printf("%-40s0x%04x\n", cert.getFieldName(i), *(int16_t*)cert.getFieldPtr(i)); i++;
            printf("%-40s0x%04x\n", cert.getFieldName(i), *(int16_t*)cert.getFieldPtr(i)); i++;

            /*
            int8_t *bytes = (int8_t*)cert.getFieldPtr(CertificateEntry::Fields::BYTES);
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
    const RelocationDir *prd = (RelocationDir*)pe.getHeaderPtr(PeFile::RELOCATION);
    if (Ident::dirExists(*prd)) {
        for (const auto &block : prd->blocks()) {
            const RelocationBase *prb = block.relocBase();
            printf("0x%08x | 0x%08x\n",
                *(int32_t*)prb->getFieldPtr(RelocationBase::PAGE_RVA),
                *(int32_t*)prb->getFieldPtr(RelocationBase::BLOCK_SIZE));
            const RelocationTable *prt = block.relocTable();
            for (int i=0; i < prt->length(); i++) {
                printf("\t0x%1x | 0x%03x\n", prt->type(i), prt->offset(i));
            }
        }
    }
}

void printDebug(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const DebugDir *pdd = (DebugDir*)pe.getHeaderPtr(PeFile::DEBUG);
    if (Ident::dirExists(*pdd)) {
        for (const auto &entry : pdd->entries()) {
            int i=0;
            for (; i < DebugEntry::MAJOR_VERSION; i++)
                printf("%-40s0x%08x\n", entry.getFieldName(i), *(int32_t*)entry.getFieldPtr(i));
            for (; i < DebugEntry::TYPE; i++)
                printf("%-40s0x%04x\n", entry.getFieldName(i), *(int16_t*)entry.getFieldPtr(i));
            for (; i < DebugEntry::_NUM_FIELDS; i++)
                printf("%-40s0x%08x\n", entry.getFieldName(i), *(int32_t*)entry.getFieldPtr(i));

            // print rsds structure
            const DebugRsds *rsds = entry.rsds();
            i=0;
            printf("\t%-40s%.4s\n", rsds->getFieldName(i),  (char*)rsds->getFieldPtr(i));

            printf("\t%-40s", rsds->getFieldName(++i));
            char *guid = (char*)rsds->getFieldPtr(i);
            for (size_t j=0; j < sizeof(((PRSDSI)nullptr)->Guid); j++)
                printf("%02x ", guid[j] & 0xFF);

            i++;
            printf("\n\t%-40s0x%08x\n", rsds->getFieldName(i), *(int32_t*)rsds->getFieldPtr(i));
            i++;
            printf("\t%-40s%s\n\n", rsds->getFieldName(i),  (char*)rsds->getFieldPtr(i));
        }
    }
}

void printArchitecture(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ArchitectureDir *pad = (ArchitectureDir*)pe.getHeaderPtr(PeFile::ARCHITECTURE);
    if (Ident::dirExists(*pad)) {
        printf("[DEBUG] '%s' has %s (RAW = 0x%08lx, diff = 0x%08lx)\n", pe.path().c_str(), pe.getHeaderName(PeFile::ARCHITECTURE), pad->dirOffset(), pad->hdrOffset() - pad->dirOffset());
    }
}

void printGlobalPointer(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const GlobalPointerDir *pgd = (GlobalPointerDir*)pe.getHeaderPtr(PeFile::GLOBAL_POINTER);
    if (Ident::dirExists(*pgd)) {
        printf("[DEBUG] '%s' has %s (RAW = 0x%08lx, diff = 0x%08lx)\n", pe.path().c_str(), pe.getHeaderName(PeFile::GLOBAL_POINTER), pgd->dirOffset(), pgd->hdrOffset() - pgd->dirOffset());
    }
}

template <typename T, const char *fmt, const Pepper::CallbacksTable<T>* (Pepper::TlsDir::*U)() const>
void printTlsStruct(const Pepper::TlsDir& tls)
{
    using namespace Pepper;
    int i=0;
    for (; i < TlsDir::SIZE_OF_ZERO_FILL; i++)
        printf(fmt, tls.getFieldName(i), *(T*)tls.getFieldPtr(i));
    for (; i < TlsDir::_NUM_FIELDS; i++)
        printf("%-40s0x%08x\n", tls.getFieldName(i), *(int32_t*)tls.getFieldPtr(i));

    printf("Callbacks Table\n");
    const CallbacksTable<T> *pct = (tls.*U)();
    for (size_t i=0; i < pct->length(); i++) {
        printf(fmt, "", pct->callbackRaw(i));
    }
}

void printTls(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const TlsDir *ptd = (TlsDir*)pe.getHeaderPtr(PeFile::TLS);
    if (Ident::dirExists(*ptd)) {
        if (Ident::is32bit(pe))
            printTlsStruct<uint32_t, fmt32, &TlsDir::cbt32>(*ptd);
        else
            printTlsStruct<uint64_t, fmt64, &TlsDir::cbt64>(*ptd);
    }
}

template <typename T, const char *fmt>
void printLdCfgStruct(const Pepper::LoadConfigDir &ldcfg)
{
    using namespace Pepper;
    int i=0;
    const void *arg = nullptr;
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::MAJOR_VERSION)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *(int32_t*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::GLOBAL_FLAGS_CLEAR)
        printf("%-40s0x%04x\n", ldcfg.getFieldName(i++), *(int16_t*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::DECOMMIT_FREE_BLOCK_THRESHOLD)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *(int32_t*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::PROCESS_HEAP_FLAGS)
        printf(fmt, ldcfg.getFieldName(i++), *(T*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::CSD_VERSION)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *(int32_t*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::EDIT_LIST)
        printf("%-40s0x%04x\n", ldcfg.getFieldName(i++), *(int16_t*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::GUARD_FLAGS)
        printf(fmt, ldcfg.getFieldName(i++), *(T*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::CODE_INTEGRITY)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *(int32_t*)arg);

    // CodeIntegrity
    if ((arg = ldcfg.getFieldPtr(i)) != nullptr) {
        printf("%-40s :\n", ldcfg.getFieldName(i++));
        int j=0;
        const CodeIntegrity *pci = (CodeIntegrity*)arg;
        for (; j < CodeIntegrity::CATALOG_OFFSET; j++)
            printf("  %-40s0x%04x\n", pci->getFieldName(j), *(int16_t*)pci->getFieldPtr(j));
        for (; j < CodeIntegrity::_NUM_FIELDS; j++)
            printf("  %-40s0x%08x\n", pci->getFieldName(j), *(int32_t*)pci->getFieldPtr(j));
    }

    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::DYNAMIC_VALUE_RELOC_TABLE_OFFSET)
        printf(fmt, ldcfg.getFieldName(i++), *(T*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::DYNAMIC_VALUE_RELOC_TABLE_SECTION)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *(int32_t*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::GUARD_RF_VERIFY_STACK_POINTER_FUNCTION_POINTER)
        printf("%-40s0x%04x\n", ldcfg.getFieldName(i++), *(int16_t*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::HOT_PATCH_TABLE_OFFSET)
        printf(fmt, ldcfg.getFieldName(i++), *(T*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::ENCLAVE_CONFIG_POINTER)
        printf("%-40s0x%08x\n", ldcfg.getFieldName(i++), *(int32_t*)arg);
    while ((arg = ldcfg.getFieldPtr(i)) != nullptr && i < LoadConfigDir::_NUM_FIELDS)
        printf(fmt, ldcfg.getFieldName(i++), *(T*)arg);
}

void printLoadConfig(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const LoadConfigDir *pld = (LoadConfigDir*)pe.getHeaderPtr(PeFile::LOAD_CONFIG);
    if (Ident::dirExists(*pld)) {
        if (Ident::is32bit(pe))
            printLdCfgStruct<int32_t, fmt32>(*pld);
        else
            printLdCfgStruct<int64_t, fmt64>(*pld);
    }
}

void printBoundImport(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const BoundImportDir *pbd = (BoundImportDir*)pe.getHeaderPtr(PeFile::BOUND_IMPORT);
    if (Ident::dirExists(*pbd)) {
        printf("[DEBUG] '%s' has %s (RAW = 0x%08lx, diff = 0x%08lx)\n", pe.path().c_str(), pe.getHeaderName(PeFile::BOUND_IMPORT), pbd->dirOffset(), pbd->hdrOffset() - pbd->dirOffset());
    }
}

template <typename T, T (Pepper::IatDir::*U)() const>
void printAddressesList(const Pepper::IatDir &id)
{
    using namespace Pepper;
    for (const auto &list : (id.*U)()) {
        for (size_t i=0; i < list.length(); i++) {
            std::cout << std::hex << "0x" << list.address(i) << '\n';
        }
        printf("\n");
    }
}

void printIat(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const IatDir *pid = (IatDir*)pe.getHeaderPtr(PeFile::IAT);
    if (Ident::dirExists(*pid)) {
        if (Ident::is32bit(pe))
            printAddressesList<
                decltype(pid->list32()),
                &IatDir::list32>(*pid);
        else
            printAddressesList<
                decltype(pid->list64()),
                &IatDir::list64>(*pid);
    }
}

void printDelayImport(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    printImportDescriptors<DelayImportDir, DelayImportDescriptor, PeFile::DELAY_IMPORT>(pe);
}

void printClrMetadata(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    ClrMetadata *pmd = (ClrMetadata*)clr.getHeaderPtr(ClrDir::CLR_METADATA);
    if (Ident::dirExists(*pmd)) {
        std::cout << "\nCLR Metadata\n";
        printf("%-40s%.4s\n", pmd->getFieldName(0), (char*)pmd->getFieldPtr(0));
        int i=1;
        for (; i <= ClrMetadata::MINOR_VERSION; i++) {
            printf("%-40s0x%04x\n", pmd->getFieldName(i), *(int16_t*)pmd->getFieldPtr(i));
        }

        for (; i <= ClrMetadata::LENGTH; i++) {
            printf("%-40s0x%04x\n", pmd->getFieldName(i), *(int32_t*)pmd->getFieldPtr(i));
        }

        printf("%-40s%s\n", pmd->getFieldName(i), (char*)pmd->getFieldPtr(i));

        for (i++; i < ClrMetadata::_NUM_FIELDS; i++) {
            printf("%-40s0x%04x\n", pmd->getFieldName(i), *(int16_t*)pmd->getFieldPtr(i));
        }

        printf("\n");
        for (const auto &stream : pmd->streams()) {
            printf("0x%08x 0x%08x %s\n",
                *(int32_t*)stream.getFieldPtr(ClrStream::OFFSET),
                *(int32_t*)stream.getFieldPtr(ClrStream::SIZE),
                 (char   *)stream.getFieldPtr(ClrStream::NAME));
        }
    }
}

void printClrResource(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrResource *prs = (ClrResource*)clr.getHeaderPtr(ClrDir::CLR_RESOURCES);
    if (Ident::dirExists(*prs)) {
        std::cout << "CLR Resource\n";
    }
}

void printClrSignature(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrSignature *psg = (ClrSignature*)clr.getHeaderPtr(ClrDir::CLR_SIGNATURE);
    if (Ident::dirExists(*psg)) {
        std::cout << "CLR Signature\n";
        for (size_t i=0; i < psg->size(); i++) {
            printf("%02x ", psg->sig()[i] & 0xFF);
        }
        printf("\n");
    }
}

void printClrCodeManager(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrCodeManager *pcm = (ClrCodeManager*)clr.getHeaderPtr(ClrDir::CLR_CODE_MANAGER);
    if (Ident::dirExists(*pcm)) {
        std::cout << "CLR Code Manager\n";
    }
}

void printClrVTableFixup(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrVTableFixup *pvt = (ClrVTableFixup*)clr.getHeaderPtr(ClrDir::CLR_VTABLE_FIXUPS);
    if (Ident::dirExists(*pvt)) {
        std::cout << "CLR VTable Fixups\n";
    }
}

void printClrExportJump(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrExportJump *pej = (ClrExportJump*)clr.getHeaderPtr(ClrDir::CLR_EXPORT_JUMP);
    if (Ident::dirExists(*pej)) {
        std::cout << "CLR Export Address Table Jumps\n";
    }
}

void printClrNativeHeader(const Pepper::ClrDir &clr)
{
    using namespace Pepper;
    const ClrNativeHeader *pnh = (ClrNativeHeader*)clr.getHeaderPtr(ClrDir::CLR_NATIVE_HEADER);
    if (Ident::dirExists(*pnh)) {
        std::cout << "CLR Native Header\n";
    }
}

void printClr(const Pepper::PeFile &pe)
{
    using namespace Pepper;
    const ClrDir *pcd = (ClrDir*)pe.getHeaderPtr(PeFile::CLR);
    if (Ident::dirExists(*pcd)) {
        int i=0;
        for (; i <= ClrDir::SIZE; i++)
            printf("%-40s0x%08x\n", pcd->getFieldName(i), *(int32_t*)pcd->getFieldPtr(i));
        for (; i <= ClrDir::MINOR_RUNTIME_VERSION; i++)
            printf("%-40s0x%04x\n", pcd->getFieldName(i), *(int16_t*)pcd->getFieldPtr(i));
        for (; i < ClrDir::_NUM_FIELDS; i++)
            printf("%-40s0x%08x\n", pcd->getFieldName(i), *(int32_t*)pcd->getFieldPtr(i));

        printf("\n");
        i=0;
        for (const auto &dataDir : pcd->directories()) {
            printf("%02d - %-32s0x%08x | 0x%08x\n", i, ClrDir::getHeaderName(i), dataDir.rva(), dataDir.size());
            i++;
        }

        printClrMetadata(*pcd);
        printClrResource(*pcd);
        printClrSignature(*pcd);
        printClrCodeManager(*pcd);
        printClrVTableFixup(*pcd);
        printClrExportJump(*pcd);
        printClrNativeHeader(*pcd);
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

