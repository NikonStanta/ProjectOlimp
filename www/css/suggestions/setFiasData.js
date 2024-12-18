/* eslint-disable */

import getData from './getData.js'
import { FIELDS } from './constants.js'

// const btnSend = document.querySelector(".btn_send");
// btnSend.addEventListener("click", sendRequest);

// async function sendRequest() {
//   const suggestions = await sendAllFields();
// }

async function sendAllFields(INPUTS_QS) {
    const fields = {
        [FIELDS.REGION]: document.querySelector(INPUTS_QS.region).value,
        [FIELDS.AREA]: document.querySelector(INPUTS_QS.area).value,
        [FIELDS.SETTLEMENT]: document.querySelector(INPUTS_QS.settlement).value,
        [FIELDS.STREET]: document.querySelector(INPUTS_QS.street).value,
        [FIELDS.HOUSE]: document.querySelector(INPUTS_QS.house).value,
    }
    const query = Object.values(fields).join(' ')
    const result = await getData(query, FIELDS.REGION, FIELDS.HOUSE)
    const { suggestions } = result

    return suggestions
}

export default sendAllFields
