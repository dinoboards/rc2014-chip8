#!/usr/bin/env node

const fs = require('fs')
const util = require('util')
const childProcess = require('child_process')
const path = require('path')
const os = require('os')

function exec(command) {
  console.log(`$: ${command}`)

  return new Promise((res, rej) => {
    const p = childProcess.spawn(command, [], {
      stdio: 'inherit',
      shell: '/bin/bash',
      cwd: "./bin"
    })

    p.on('close', code => code ? rej(new Error(`Exit with code ${code}`)) : res())
  })
}


async function test(sampleName) {
  if (fs.existsSync(`./bin/${sampleName}.ch8`))
    fs.unlinkSync(`./bin/${sampleName}.ch8`)
  await exec(`cpm chip8asm ${sampleName}.cas`)

  const compiledFile = fs.readFileSync(`./bin/${sampleName}.ch8`);
  const expected = fs.readFileSync(`./test-samples/${sampleName}.ch8`);

  const result = Buffer.compare(compiledFile, expected);
  if (result != 0) {
    console.log(`\u001b[31mAssembly failed to produce correct output for ${sampleName}\u001b[0m`);
    process.nextTick(() => process.exit(-1));
  } else {
    fs.unlinkSync(`./bin/${sampleName}.ch8`)
    fs.unlinkSync(`./bin/${sampleName}.cas`)
  }
}

async function main() {
  await test('draw');
}

main()
