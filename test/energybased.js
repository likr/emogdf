/* eslint-env mocha */

const {run} = require('./util')

run((ogdf) => {
  const {
    FMMMLayout
  } = ogdf
  describe('FMMMLayout', () => {
    describe('useHighLevelOptions(value)', () => {
      it('sets parameter', () => {
        const layout = new FMMMLayout()
        layout.useHighLevelOptions(true)
      })
    })

    describe('unitEdgeLength(value)', () => {
      it('sets parameter', () => {
        const layout = new FMMMLayout()
        layout.unitEdgeLength(1.0)
      })
    })

    describe('newInitialPlacement(value)', () => {
      it('sets parameter', () => {
        const layout = new FMMMLayout()
        layout.newInitialPlacement(true)
      })
    })

    describe('randSeed(value)', () => {
      it('sets parameter', () => {
        const layout = new FMMMLayout()
        layout.randSeed(0)
      })
    })
  })
})
