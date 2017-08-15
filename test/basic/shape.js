/* eslint-env mocha */

const assert = require('power-assert')
const {run} = require('../util')

run((ogdf) => {
  const {
    Shape
  } = ogdf

  describe('Shape', () => {
    const properties = [
      'Rect',
      'RoundedRect',
      'Ellipse',
      'Triangle',
      'Pentagon',
      'Hexagon',
      'Octagon',
      'Rhomb',
      'Trapeze',
      'Parallelogram',
      'InvTriangle',
      'InvTrapeze',
      'InvParallelogram',
      'Image'
    ]
    for (const name of properties) {
      it(`has enum ${name}`, () => {
        assert.notEqual(Shape[name].value, undefined)
      })
    }
  })
})
