sudo mkdir -p /mnt/kvs
mount -t tmpfs -o size=64G tmpfs /mnt/kvs
for i in {0..3}; do
    sudo dd if=/dev/zero of=/mnt/kvs/disk$i.img bs=1M seek=9999 count=1
    sudo losetup /dev/loop$i /mnt/kvs/disk$i.img
done
