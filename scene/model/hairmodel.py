import json
import numpy as np

cell_size = np.float64(5)
def Nano_S(length):
    return length/cell_size

LambdaRange = np.arange(Nano_S(380),Nano_S(700), Nano_S(cell_size)); 
WaveAngleRange = np.arange(-90, 0, 5);
#model data
model = {
    "name": "hairmodel",
    "field":{
        "type": "2d",
        "size[nm]": [8000, 8000],
        "cell_size[nm]": cell_size,
        "boundary_size": 10,
        #other parameters
    },
    "use_model":{
        "type": "FazzyHair_incidenceLayerModel",
    },
    "WaveParameter":{
        "LambdaRange":LambdaRange,
        "WaveAngleRange":WaveAngleRange,
    },
}

class NumpyEncoder(json.JSONEncoder):
    """ Special json encoder for numpy types """
    def default(self, obj):
        if isinstance(obj, np.integer):
            return int(obj)
        elif isinstance(obj, np.floating):
            return float(obj)
        elif isinstance(obj, np.ndarray):
            return obj.tolist()
        return json.JSONEncoder.default(self, obj)

with open('scene/model/hairmodel.json', 'w') as outfile:
    json.dump(model, outfile,cls = NumpyEncoder)