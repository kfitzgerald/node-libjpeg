var fs  = require('fs');
var Jpeg = require('../').Jpeg;
var Buffer = require('buffer').Buffer;

var yuv = fs.readFileSync(__dirname + '/yuv420-example.yuv');

var jpeg = new Jpeg(yuv, 640, 480, 'yuv420');
var jpeg_img = jpeg.encodeSync().toString('binary');

fs.writeFileSync(__dirname + '/yuv-jpeg.jpeg', jpeg_img, 'binary');
