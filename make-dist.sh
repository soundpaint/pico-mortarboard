#/bin/bash
cd "$(dirname "$0")"
project_name=$(basename ${PWD})
project_path=${project_name}
tgz_date=`date +%Y-%m-%d_%H-%M-%S`
tgz_prefix=${project_name}\_${tgz_date}
cd ..
tar cvf ./${tgz_prefix}.tar.bz2 \
  --transform=s/${project_path}/${tgz_prefix}/ \
  --bzip \
  ${project_path}/{CMakeLists.txt,*.sh,*.cmake,src/*.{hpp,cpp},test/config/{Makefile,*.{hpp,cpp},pico/*.h,hardware/*.{h,c}}}
