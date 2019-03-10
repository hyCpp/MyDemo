cd /home/webcloud/navicloud
echo ". build/envsetup.sh"
. build/envsetup.sh
echo "lunch "$1
lunch $1
echo "make libvoicerecog-navi "$2
make libvoicerecog-navi $2
echo "end."
