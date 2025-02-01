import json

#model data
filename = "hairmodel"
model = {
    "name": filename,
    "field":{
        "type": "2d",
        "size[nm]": [8000, 8000],
        "cell_size[nm]": 10,
        "boundary_size": 10,
        #other parameters
    },
    "use_model":{
        "type": "HairMultilayerModel",
    },
    "WaveParameter":{
        "LambdaRange":[400,700,5],
        "WaveAngleRange":[-90,0,1],
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

with open('scene/model/'+filename+'.json', 'w') as outfile:
    json.dump(model, outfile,cls = NumpyEncoder)