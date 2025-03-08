#!/bin/bash
# 命令1
echo "HairPretteMultilayerPerlinModel"
./build/NS-FDTD-CLI -m scene/model/HairPretteMultilayerPerlinModel.json

echo "hairmodelDamagedLargeDisPerlin"
./build/NS-FDTD-CLI -m scene/model/hairmodelDamagedLargeDisPerlin.json

echo "HairPretteMultilayerPerlinModel"
./build/NS-FDTD-CLI -m scene/model/HairPretteMultilayerPerlinModel.json

echo "HairMultilayerDamagedLackLayerPerlinModel"
./build/NS-FDTD-CLI -m scene/model/HairMultilayerDamagedLackLayerPerlinModel.json

echo "HairMultilayerDamagedTiltPerlinModel"
./build/NS-FDTD-CLI -m scene/model/HairMultilayerDamagedTiltPerlinModel.json

echo "HairPretteMultilayerPerlinModel"
./build/NS-FDTD-CLI -m scene/model/HairPretteMultilayerPerlinModel.json

echo "hairmodelRepairedPerlin"
./build/NS-FDTD-CLI -m scene/model/hairmodelRepairedPerlin.json
# 脚本结束
echo "complete"