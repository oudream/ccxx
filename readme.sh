#!/usr/bin/env bash

### notice 注意 :
### notice 注意 :
# 全局配置邮件发送服务 : smtp 时参考 : https://www.jianshu.com/p/51ef5e012b25
# git pull 下来的项目默认存放在 ~/.jenkins/workspace/
# 在 JOB 中配置时，${WORKSPACE}是指 ~/.jenkins/workspace/projectName
# 在 JOB 中配置xUnit -> google test -> Pattern 用的是相对路径。例如：cmake-gcc/unittest-report
### notice 注意 .


### jenkins install :
wget http://mirrors.jenkins.io/war-stable/latest/jenkins.war && \
java -jar jenkins.war --httpPort=2280
### jenkins install .


### jenkins run on docker :
docker run \
  -u root \
  --rm \
  -d \
  -p 8080:8080 \
  -p 50000:50000 \
  -v jenkins-data:/var/jenkins_home \
  -v /var/run/docker.sock:/var/run/docker.sock \
  jenkinsci/blueocean
### jenkins run on docker .


### c++ 插件 :
valgrind
cmake builder
xunit
cppcheck
warnings
### c++ 插件 .


### c++ 单元测试例子 :
git clone https://github.com/oudream/ccxx.git

H/2 * * * *

cd /opt/ddd/ccpp/ccxx
cmake . -DCMAKE_BUILD_TYPE=Debug --build "/opt/ddd/ccpp/ccxx" -B"/opt/ddd/ccpp/ccxx/build/cmake-gcc"
cd build/cmake-gcc && make
export GTEST_OUTPUT="xml:/opt/ddd/ccpp/ccxx/build/cmake-gcc/unittest-report"
make test


cd $WORKSPACE
cmake . -DCMAKE_BUILD_TYPE=Debug --build "${WORKSPACE}" -B"${WORKSPACE}/build/cmake-gcc"
export GTEST_OUTPUT="xml:${WORKSPACE}/build/cmake-gcc/unittest-report"
cd ${WORKSPACE}/cmake-gcc
make
make test
### c++ 单元测试例子 .


### Jenkinsfile 例子 :
http://senlinzhan.github.io/2017/10/08/gtest/
### Jenkinsfile 例子 .

### cmake : gtest_add_tests
# https://cmake.org/cmake/help/v3.10/module/GoogleTest.html
