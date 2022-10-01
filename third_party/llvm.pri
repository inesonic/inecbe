##-*-makefile-*-########################################################################################################
# Copyright 2016 - 2022 Inesonic, LLC
# 
# This file is licensed under two licenses.
#
# Inesonic Commercial License, Version 1:
#   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
#   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
#   strictly prohibited.
#
# GNU Public License, Version 2:
#   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
#   version.
#   
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#   details.
#   
#   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
#   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
########################################################################################################################

include("llvm_headers_only.pri")

unix {
    !defined(LLVM_LIBDIR, var) {
        LLVM_LIBDIR=$${LLVM_INSTALL}/lib
    }

    LIBS += -L$${LLVM_LIBDIR}

    !macx {
        include("curses.pri")
    }

    contains(LLVM_TARGETS, X86) {
        LIBS += -lLLVMX86AsmParser
    }

    contains(LLVM_TARGETS, ARM) {
        LIBS += -lLLVMARMAsmParser
    }

    contains(LLVM_TARGETS, RISCV) {
        LIBS += -lLLVMRISCVAsmParser
    }

    contains(LLVM_TARGETS, AVR) {
        LIBS += -lLLVMAVRAsmParser
    }

    LIBS += -lLLVMBitstreamReader
    LIBS += -lLLVMFrontendOpenMP
    LIBS += -lLLVMCFGuard
    LIBS += -lLLVMPasses
    LIBS += -lLLVMTextAPI
    LIBS += -lLLVMRemarks

    contains(LLVM_TARGETS, X86) {
#        LIBS += -lLLVMX86AsmParser
        LIBS += -lLLVMX86Desc
        LIBS += -lLLVMX86Info
        LIBS += -lLLVMX86Disassembler
        LIBS += -lLLVMX86Info
        LIBS += -lLLVMX86CodeGen
    }

    contains(LLVM_TARGETS, ARM) {
#        LIBS += -lLLVMARMAsmParser
        LIBS += -lLLVMARMUtils
        LIBS += -lLLVMARMDesc
        LIBS += -lLLVMARMInfo
        LIBS += -lLLVMARMDisassembler
        LIBS += -lLLVMARMInfo
        LIBS += -lLLVMARMCodeGen
    }

    contains(LLVM_TARGETS, RISCV) {
#        LIBS += -lLLVMARMAsmParser
        LIBS += -lLLVMRISCVDesc
        LIBS += -lLLVMRISCVInfo
        LIBS += -lLLVMRISCVDisassembler
        LIBS += -lLLVMRISCVInfo
        LIBS += -lLLVMRISCVCodeGen
    }

    contains(LLVM_TARGETS, ARC) {
        LIBS += -lLLVMARCCodeGen
        LIBS += -lLLVMARCDesc
        LIBS += -lLLVMARCDisassembler
        LIBS += -lLLVMARCInfo
    }

    contains(LLVM_TARGETS, AVR) {
        LIBS += -lLLVMAVRCodeGen
        LIBS += -lLLVMAVRDesc
        LIBS += -lLLVMAVRDisassembler
        LIBS += -lLLVMAVRInfo
    }

    LIBS += -lLLVMAnalysis
    LIBS += -lLLVMCodeGen
    LIBS += -lLLVMCore
    LIBS += -lLLVMipo
    LIBS += -lLLVMInstCombine
    LIBS += -lLLVMInstrumentation
    LIBS += -lLLVMMC
    LIBS += -lLLVMMCParser
    LIBS += -lLLVMObjCARCOpts
    LIBS += -lLLVMOption
    LIBS += -lLLVMScalarOpts
    LIBS += -lLLVMSupport
    LIBS += -lLLVMTransformUtils
    LIBS += -lLLVMVectorize
    LIBS += -lclangBasic
    LIBS += -lclangCodeGen
    LIBS += -lclangDriver
    LIBS += -lclangFrontend
    LIBS += -lclangFrontendTool
    LIBS += -lclangToolingCore
    LIBS += -lPolly
    LIBS += -lLLVMAsmPrinter
    LIBS += -lLLVMDebugInfoCodeView
    LIBS += -lLLVMDebugInfoMSF
    LIBS += -lLLVMGlobalISel
    LIBS += -lLLVMSelectionDAG

#    LIBS += -lLLVMX86Utils
    LIBS += -lLLVMMCDisassembler
    LIBS += -lclangCodeGen
    LIBS += -lLLVMCoroutines
    LIBS += -lLLVMCoverage
    LIBS += -lLLVMLTO
    LIBS += -lLLVMObjCARCOpts
    LIBS += -lclangRewriteFrontend
    LIBS += -lclangARCMigrate
    LIBS += -lclangStaticAnalyzerFrontend
    LIBS += -lclangFrontend
    LIBS += -lclangDriver
    LIBS += -lLLVMOption
    LIBS += -lclangParse
    LIBS += -lclangSerialization
    LIBS += -lclangSema
    LIBS += -lclangEdit
    LIBS += -lclangStaticAnalyzerCheckers
    LIBS += -lclangASTMatchers
    LIBS += -lclangStaticAnalyzerCore
    LIBS += -lclangAnalysis
    LIBS += -lclangAST
    LIBS += -lclangRewrite
    LIBS += -lclangLex
    LIBS += -lclangBasic
    LIBS += -lLLVMPasses
    LIBS += -lLLVMCodeGen
    LIBS += -lLLVMTarget
    LIBS += -lLLVMipo
    LIBS += -lLLVMBitWriter
    LIBS += -lLLVMIRReader
    LIBS += -lLLVMAsmParser
    LIBS += -lLLVMLinker
    LIBS += -lLLVMInstrumentation
    LIBS += -lLLVMScalarOpts
    LIBS += -lLLVMInstCombine
    LIBS += -lLLVMVectorize
    LIBS += -lLLVMTransformUtils
    LIBS += -lLLVMAnalysis
    LIBS += -lLLVMObject
    LIBS += -lLLVMMCParser
    LIBS += -lLLVMMC
    LIBS += -lLLVMBitReader
    LIBS += -lLLVMProfileData
    LIBS += -lLLVMCore
    LIBS += -lLLVMBinaryFormat
    LIBS += -lLLVMSupport
    LIBS += -lLLVMDemangle
    LIBS += -lPollyISL

    LIBS += -lclangCrossTU
    LIBS += -lclangIndex
    LIBS += -lLLVMAggressiveInstCombine

    LIBS += -lLLVMDebugInfoDWARF

    !contains(DEFINES, CBE_EXTERNAL_LINKER) {
#        LIBS += -llldReaderWriter
        LIBS += -lLLVMSupport

        LIBS += -llldCommon

#        LIBS += -llldCore
#        LIBS += -llldDriver
#        LIBS += -llldYAML

        macx {
            LIBS += -llldMachO
        } else {
            LIBS += -llldELF
            LIBS += -lLLVMObject
        }
    }
}

win32 {
    !defined(LLVM_LIBDIR, var) {
        LLVM_LIBDIR = $${LLVM_INSTALL}/lib
    }

    contains(LLVM_TARGETS, X86) {
        LIBS += $${LLVM_LIBDIR}/LLVMX86AsmParser.lib
        LIBS += $${LLVM_LIBDIR}/LLVMX86Desc.lib
        LIBS += $${LLVM_LIBDIR}/LLVMX86Info.lib
        LIBS += $${LLVM_LIBDIR}/LLVMX86Disassembler.lib
        LIBS += $${LLVM_LIBDIR}/LLVMX86Info.lib
        LIBS += $${LLVM_LIBDIR}/LLVMX86CodeGen.lib
    }

    contains(LLVM_TARGETS, ARM) {
        LIBS += $${LLVM_LIBDIR}/LLVMARMUtils.lib
        LIBS += $${LLVM_LIBDIR}/LLVMARMDesc.lib
        LIBS += $${LLVM_LIBDIR}/LLVMARMInfo.lib
        LIBS += $${LLVM_LIBDIR}/LLVMARMDisassembler.lib
        LIBS += $${LLVM_LIBDIR}/LLVMARMInfo.lib
        LIBS += $${LLVM_LIBDIR}/LLVMARMCodeGen.lib
    }

    contains(LLVM_TARGETS, RISCV) {
        LIBS += $${LLVM_LIBDIR}/LLVMRISCVDesc.lib
        LIBS += $${LLVM_LIBDIR}/LLVMRISCVInfo.lib
        LIBS += $${LLVM_LIBDIR}/LLVMRISCVDisassembler.lib
        LIBS += $${LLVM_LIBDIR}/LLVMRISCVInfo.lib
        LIBS += $${LLVM_LIBDIR}/LLVMRISCVCodeGen.lib
    }

    contains(LLVM_TARGETS, ARC) {
        LIBS += $${LLVM_LIBDIR}/LLVMARCCodeGen.lib
        LIBS += $${LLVM_LIBDIR}/LLVMARCDesc.lib
        LIBS += $${LLVM_LIBDIR}/LLVMARCDisassembler.lib
        LIBS += $${LLVM_LIBDIR}/LLVMARCInfo.lib
    }

    contains(LLVM_TARGETS, AVR) {
        LIBS += $${LLVM_LIBDIR}/LLVMAVRCodeGen.lib
        LIBS += $${LLVM_LIBDIR}/LLVMAVRDesc.lib
        LIBS += $${LLVM_LIBDIR}/LLVMAVRDisassembler.lib
        LIBS += $${LLVM_LIBDIR}/LLVMAVRInfo.lib
    }

    LIBS += $${LLVM_LIBDIR}/LLVMBitstreamReader.lib
    LIBS += $${LLVM_LIBDIR}/LLVMFrontendOpenMP.lib
    LIBS += $${LLVM_LIBDIR}/LLVMCFGuard.lib
    LIBS += $${LLVM_LIBDIR}/LLVMPasses.lib
    LIBS += $${LLVM_LIBDIR}/LLVMTextAPI.lib
    LIBS += $${LLVM_LIBDIR}/LLVMRemarks.lib

    LIBS += $${LLVM_LIBDIR}/LLVMAnalysis.lib
    LIBS += $${LLVM_LIBDIR}/LLVMCodeGen.lib
    LIBS += $${LLVM_LIBDIR}/LLVMCore.lib
    LIBS += $${LLVM_LIBDIR}/LLVMipo.lib
    LIBS += $${LLVM_LIBDIR}/LLVMInstCombine.lib
    LIBS += $${LLVM_LIBDIR}/LLVMInstrumentation.lib
    LIBS += $${LLVM_LIBDIR}/LLVMMC.lib
    LIBS += $${LLVM_LIBDIR}/LLVMMCParser.lib
    LIBS += $${LLVM_LIBDIR}/LLVMObjCARCOpts.lib
    LIBS += $${LLVM_LIBDIR}/LLVMOption.lib
    LIBS += $${LLVM_LIBDIR}/LLVMScalarOpts.lib
    LIBS += $${LLVM_LIBDIR}/LLVMTransformUtils.lib
    LIBS += $${LLVM_LIBDIR}/LLVMVectorize.lib
    LIBS += $${LLVM_LIBDIR}/clangBasic.lib
    LIBS += $${LLVM_LIBDIR}/clangCodeGen.lib
    LIBS += $${LLVM_LIBDIR}/clangDriver.lib
    LIBS += $${LLVM_LIBDIR}/clangFrontend.lib
    LIBS += $${LLVM_LIBDIR}/clangFrontendTool.lib
    LIBS += $${LLVM_LIBDIR}/clangToolingCore.lib
#    LIBS += $${LLVM_LIBDIR}/Polly.lib
    LIBS += $${LLVM_LIBDIR}/LLVMAsmPrinter.lib
    LIBS += $${LLVM_LIBDIR}/LLVMDebugInfoCodeView.lib
    LIBS += $${LLVM_LIBDIR}/LLVMDebugInfoMSF.lib
    LIBS += $${LLVM_LIBDIR}/LLVMGlobalISel.lib
    LIBS += $${LLVM_LIBDIR}/LLVMSelectionDAG.lib

#    equals(LLVM_VERSION,100000) {
#        LIBS += $${LLVM_LIBDIR}/LLVMX86AsmPrinter.lib
#    }

    LIBS += $${LLVM_LIBDIR}/LLVMMCDisassembler.lib
    LIBS += $${LLVM_LIBDIR}/LLVMCoroutines.lib
    LIBS += $${LLVM_LIBDIR}/LLVMCoverage.lib
    LIBS += $${LLVM_LIBDIR}/LLVMLTO.lib
    LIBS += $${LLVM_LIBDIR}/LLVMOption.lib
    LIBS += $${LLVM_LIBDIR}/clangCodeGen.lib
    LIBS += $${LLVM_LIBDIR}/clangRewriteFrontend.lib
    LIBS += $${LLVM_LIBDIR}/clangARCMigrate.lib
    LIBS += $${LLVM_LIBDIR}/clangStaticAnalyzerFrontend.lib
    LIBS += $${LLVM_LIBDIR}/clangFrontend.lib
    LIBS += $${LLVM_LIBDIR}/clangDriver.lib
    LIBS += $${LLVM_LIBDIR}/clangParse.lib
    LIBS += $${LLVM_LIBDIR}/clangSerialization.lib
    LIBS += $${LLVM_LIBDIR}/clangSema.lib
    LIBS += $${LLVM_LIBDIR}/clangEdit.lib
    LIBS += $${LLVM_LIBDIR}/clangStaticAnalyzerCheckers.lib
    LIBS += $${LLVM_LIBDIR}/clangASTMatchers.lib
    LIBS += $${LLVM_LIBDIR}/clangStaticAnalyzerCore.lib
    LIBS += $${LLVM_LIBDIR}/clangAnalysis.lib
    LIBS += $${LLVM_LIBDIR}/clangAST.lib
    LIBS += $${LLVM_LIBDIR}/clangRewrite.lib
    LIBS += $${LLVM_LIBDIR}/clangLex.lib
    LIBS += $${LLVM_LIBDIR}/clangBasic.lib
    LIBS += $${LLVM_LIBDIR}/LLVMPasses.lib
    LIBS += $${LLVM_LIBDIR}/LLVMTarget.lib
    LIBS += $${LLVM_LIBDIR}/LLVMipo.lib
    LIBS += $${LLVM_LIBDIR}/LLVMBitWriter.lib
    LIBS += $${LLVM_LIBDIR}/LLVMIRReader.lib
    LIBS += $${LLVM_LIBDIR}/LLVMAsmParser.lib
    LIBS += $${LLVM_LIBDIR}/LLVMLinker.lib
    LIBS += $${LLVM_LIBDIR}/LLVMInstrumentation.lib
    LIBS += $${LLVM_LIBDIR}/LLVMScalarOpts.lib
    LIBS += $${LLVM_LIBDIR}/LLVMInstCombine.lib
    LIBS += $${LLVM_LIBDIR}/LLVMVectorize.lib
    LIBS += $${LLVM_LIBDIR}/LLVMTransformUtils.lib
    LIBS += $${LLVM_LIBDIR}/LLVMAnalysis.lib
    LIBS += $${LLVM_LIBDIR}/LLVMObject.lib
    LIBS += $${LLVM_LIBDIR}/LLVMMCParser.lib
    LIBS += $${LLVM_LIBDIR}/LLVMMC.lib
    LIBS += $${LLVM_LIBDIR}/LLVMBitReader.lib
    LIBS += $${LLVM_LIBDIR}/LLVMProfileData.lib
    LIBS += $${LLVM_LIBDIR}/LLVMCore.lib
    LIBS += $${LLVM_LIBDIR}/LLVMBinaryFormat.lib
    LIBS += $${LLVM_LIBDIR}/LLVMSupport.lib
    LIBS += $${LLVM_LIBDIR}/LLVMDemangle.lib
#    LIBS += $${LLVM_LIBDIR}/PollyISL.lib

    LIBS += $${LLVM_LIBDIR}/clangCrossTU.lib
    LIBS += $${LLVM_LIBDIR}/clangIndex.lib
    LIBS += $${LLVM_LIBDIR}/LLVMAggressiveInstCombine.lib

    LIBS += $${LLVM_LIBDIR}/LLVMDebugInfoDWARF.lib
    LIBS += $${LLVM_LIBDIR}/LLVMDebugInfoMSF.lib

    !contains(DEFINES, CBE_EXTERNAL_LINKER) {
        LIBS += $${LLVM_LIBDIR}/LLVMDebugInfoPDB.lib
        LIBS += $${LLVM_LIBDIR}/LLVMLibDriver.lib
#        LIBS += $${LLVM_LIBDIR}/lldReaderWriter.lib

        LIBS += $${LLVM_LIBDIR}/lldCommon.lib

#        LIBS += $${LLVM_LIBDIR}/lldCore.lib
#        LIBS += $${LLVM_LIBDIR}/lldDriver.lib
#        LIBS += $${LLVM_LIBDIR}/lldYAML.lib
        LIBS += $${LLVM_LIBDIR}/lldCOFF.lib
    }
}
