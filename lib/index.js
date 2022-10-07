const binary = require("@mapbox/node-pre-gyp"),
  path = require("node:path");

const binding_path = binary.find(
  path.resolve(path.join(__dirname, "../package.json"))
);
const mod = require(binding_path);

module.exports = mod.registerProtocolHandler;
