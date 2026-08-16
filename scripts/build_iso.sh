#!/bin/bash
# DECLARATIONS #
ARCH=i386
IMAGE_LOC="../kernel/arch/$ARCH/build/fausto"
ISO_BUILD_DIR="../isodir"

# FUNCTIONS #
log() {
	p=""
	if [ "$2" == "!" ]; then
		p="[!]"
	fi
	echo "=>$p $1"
}
fail_count=0
check() {
	if [ $? != 0 ]; then
		echo "#<- FAILED ->#"
		((fail_count++))
	fi
}

# MAIN #
log "Building for arch '$ARCH'"

if [ ! -f "$IMAGE_LOC" ]; then
	log "Image not found -> $IMAGE_LOC" !
	exit 1
fi

if [ -d "$ISO_BUILD_DIR" ]; then
	log "ISO build directory not found, creating"
	mkdir -p $ISO_BUILD_DIR
fi

log "Creating grub directory"
mkdir -p "$ISO_BUILD_DIR/boot/grub"
check

log "Copying image"
cp $IMAGE_LOC "$ISO_BUILD_DIR/boot/"
check

log "Copying grub configuration"
GRUB_CFG_LOC="../kernel/arch/$ARCH/grub.cfg"
if [ ! -f $GRUB_CFG_LOC ]; then
	log "No grub configuration found -> $GRUB_CFG_LOC" "!"
	exit 1
fi
cp $GRUB_CFG_LOC "$ISO_BUILD_DIR/boot/grub/"
check

log "Creating ISO file"
grub-mkrescue -o "$ISO_BUILD_DIR/fausto.iso" $ISO_BUILD_DIR
check

echo "Done. $fail_count unit failed"
