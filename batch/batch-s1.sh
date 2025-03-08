#!/bin/bash

# 定义要执行的模型数组
models=(
  "HairMultilayerPerlinModel"
  "HairPretteMultilayerPerlinModel"
)

# 计算模型数量
num_models=${#models[@]}

# 循环执行模型，每次两个，并发执行
for ((i=0; i<num_models; i+=2)); do
  # 执行第一个模型 (后台运行)
  echo "${models[$i]}"
  ../build/NS-FDTD-CLI -m ../scene/model/${models[$i]}.json &
  process1_pid=$! # 获取第一个后台进程的 PID

  # 检查是否还有第二个模型，并执行 (后台运行)
  if (( i+1 < num_models )); then
    echo "${models[$i+1]}"
    ../build/NS-FDTD-CLI -m ../scene/model/${models[$i+1]}.json &
    process2_pid=$! # 获取第二个后台进程的 PID

    # 等待两个进程都执行完成
    wait "$process1_pid" "$process2_pid"
  else
    # 如果只有第一个进程，等待它完成
    wait "$process1_pid"
  fi
done

echo "complete"

