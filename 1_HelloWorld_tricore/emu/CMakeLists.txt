
# Prepare some useful variable for use in config files
set(TGT_ELF "${TGT}.elf")
set(ISODIR "${PROJECT_BINARY_DIR}/grub_iso")
set(FAIL_BINARY "$ENV{EZS_BASE_DIR}/fail/fail-client")
set(ISOFILE "${PROJECT_BINARY_DIR}/${TGT}.iso")

# Setup Bochs configuration
set(BOCHS_BIOS "${CMAKE_CURRENT_SOURCE_DIR}/BIOS-bochs-latest")
set(BOCHS_GUEST_RAM 32)
set(BOCHS_HOST_RAM 32)
set(BOCHS_VGA_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/vgabios.bin")
set(BOCHS_ISO_IMAGE "${ISOFILE}")

set(STARTUP_SCRIPTS ${PROJECT_BINARY_DIR}/startup_scripts)
file(MAKE_DIRECTORY ${STARTUP_SCRIPTS})
set(BOCHSRC "${STARTUP_SCRIPTS}/bochsrc")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bochsrc.in" ${BOCHSRC} )
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bochsrc-gdb.in" "${BOCHSRC}-gdb" )
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/bochsrc-term.in" "${BOCHSRC}-term" )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qrun.sh.in ${STARTUP_SCRIPTS}/qrun.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/frun.sh.in ${STARTUP_SCRIPTS}/frun.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pts.sh.in ${STARTUP_SCRIPTS}/pts.sh)
configure_file(${SCRIPT_DIR}/findport.pl ${STARTUP_SCRIPTS}/findport.pl)

message(STATUS "Fail* @ ${FAIL_BINARY}")
#set(GDBINITFILE ${STARTUP_SCRIPTS}/.gdbinit)
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/gdbinit.in" ${GDBINITFILE})

# Grub
find_program(GRUB_MKRESCUE "grub-mkrescue")
if(NOT GRUB_MKRESCUE)
		message(FATAL_ERROR "grub-mkrescue not found, cannot create bootable iso :(")
endif()


message(STATUS "Building bootable ISO for ${TGT}")

# Generate build directory for grub-mkrescue
set(BOOTDIR "${ISODIR}/boot/")
set(GRUBDIR "${BOOTDIR}/grub")
file(MAKE_DIRECTORY ${GRUBDIR})

# Copy and configure grub configuration file
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/grub.cfg.in" "${GRUBDIR}/grub.cfg")

# Create bootable ISO from TGT
add_custom_command(
		DEPENDS ${TGT}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TGT}> ${BOOTDIR}/${TGT_ELF}
		COMMAND ${GRUB_MKRESCUE}
		ARGS -o ${ISOFILE} ${ISODIR}
        OUTPUT ${ISOFILE} ${BOOTDIR}/${TGT_ELF}
		COMMENT "Generating bootable grub iso."
		)

# Create bootable ISO
add_custom_target( iso DEPENDS ${ISOFILE} )

# Start FAIL
add_custom_target( sim
        COMMAND ${STARTUP_SCRIPTS}/frun.sh
		DEPENDS ${ISOFILE}
		COMMENT "Starting Fail*"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		)

# Start qemu
add_custom_target(run
		DEPENDS ${ISOFILE} ${TGT}
		COMMAND echo "Running ${ISOFILE}..."
        COMMAND ${STARTUP_SCRIPTS}/qrun.sh
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		)

add_custom_target( ddd
# Start Qemu in Debug mode.
        DEPENDS ${TGT}
        COMMAND ${STARTUP_SCRIPTS}/qrun.sh ddd
		COMMENT "Debugging in Qemu"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_custom_target( gdb
# Start Qemu in Debug mode.
        DEPENDS ${TGT}
        COMMAND ${STARTUP_SCRIPTS}/qrun.sh gdb
		COMMENT "Debugging in Qemu"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_custom_target( cgdb
# Start Qemu in Debug mode.
        DEPENDS ${TGT}
        COMMAND ${STARTUP_SCRIPTS}/qrun.sh cgdb
		COMMENT "Debugging in Qemu"
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
