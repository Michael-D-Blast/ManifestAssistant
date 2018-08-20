#! /bin/bash

working_dir=`pwd`
release_dir=${working_dir}/release

print_usage ()
{
	cat << EOT

	Usage: $(basename $0) <target_exe>
	
EOT
}

[[ $# -ne 1 ]] && print_usage && exit 1

exe=$1

[[ -d $release_dir ]] && rm -rf $release_dir
mkdir $release_dir

ldd $exe |awk '{print $3}' |grep so | while read line
do
	cp $line -vi $release_dir
done

cp $exe -vi $release_dir

tar -czvf ${exe}_$(uname -m).tgz $release_dir
