const test = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const flows = JSON.parse(fs.readFileSync(path.join(__dirname, '..', 'flows.json')));
const formatters = flows.filter(n => n.type === 'function' && n.func.includes('node-red-contrib-influxdb expects'));
function sample() {
  return {payload: {sensor: {device: 'test', temperature: 20, humidity: 50, smoke: 0},
    weather: {temperature: 25, humidity: 50, windSpeed: 5},
    risk: {score: 0, level: 'BAIXO', shouldAlert: false},
    location: {city: 'Manaus', state: 'Amazonas', abbr: 'AM'}, timestamp: '2026-01-01T00:00:00Z'}};
}
test('both writers emit numeric fields, tags and a Date timestamp', () => {
  assert.equal(formatters.length, 2);
  for (const f of formatters) {
    const msg = new Function('msg', f.func)(sample());
    assert.equal(msg.payload[0].risk_score, 0);
    assert.equal(msg.payload[1].state, 'AM');
    assert.ok(msg.payload[0].time instanceof Date);
    assert.equal(msg.measurement, 'fire_detection');
  }
});
test('invalid measurements are rejected', () => {
  for (const f of formatters) {
    const msg = sample(); msg.payload.sensor.temperature = 'invalid';
    assert.throws(() => new Function('msg', f.func)(msg));
  }
});
test('all functions are syntactically valid', () => {
  for (const f of flows.filter(n => n.type === 'function')) {
    new Function('msg', 'node', 'global', 'context', f.func);
  }
});
test('external I/O and timers require explicit activation', () => {
  const types = ['influxdb out','mqtt in','openweathermap','node-red-contrib-whatsapp-cmb-send-message','inject'];
  for (const f of flows.filter(n => types.includes(n.type))) assert.equal(f.d, true);
});

test('zero weather values stay zero and missing observations fail', () => {
  const scorer = flows.find(n => n.name === 'Calcular Risco API (Meteorológico)');
  const run = new Function('msg', 'node', scorer.func);
  const node = {status() {}};
  const message = {payload: {tempc: 0, humidity: 0, windspeed: 0}, location: {city: 'test', abbr: 'XX'}};
  const output = run(message, node)[0];
  assert.equal(output.payload.weather.temperature, 0);
  assert.equal(output.payload.weather.humidity, 0);
  assert.throws(() => run({payload: {}, location: {}}, node));
});
test('sensor scoring does not fabricate readings when context is empty', () => {
  const scorer = flows.find(n => n.name === 'Calcular Risco de Queimada');
  assert.throws(() => new Function('msg', 'node', 'global', scorer.func)({}, {}, {get() {return null;}}));
});
