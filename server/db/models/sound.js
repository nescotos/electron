//Using ottoman
var ottoman = require('ottoman');
//Creatint the document
var SoundModel = ottoman.model('Sound', {
    metricId: {
        type: 'string',
        auto: 'uuid',
        readonly: true
    },
    createdAt: {
        type: 'Date',
        default: function() {
            return new Date() //Now date
        }
    },
    metric: 'number',
    sensorId: 'string'
}, {
    index: {
        findByMetricId: {
            type: 'refdoc',
            by: 'metricId'
        },
        findBySensorId: {
            by: 'sensorId'
        }
    }
});

//Saving the document
SoundModel.createAndSave = function(metric, sensorId, done){
  this.create({ metric : metric, sensorId : sensorId }, done);
}


module.exports = SoundModel;
