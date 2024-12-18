import processData from './processData.js'

window.addEventListener('DOMContentLoaded', () => {
    const addressVars = ['addr_reg', 'addr_post']

    let currAddress = addressVars[0]
    processData(currAddress)

    currAddress = addressVars[1]
    processData(currAddress)
})
