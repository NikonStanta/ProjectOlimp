/* eslint-disable */

import { URL, TOKEN } from "./constants.js";

async function getData(query, from_bound, to_bound, location) {
  var options = {
    method: "POST",
    mode: "cors",
    headers: {
      "Content-Type": "application/json",
      Accept: "application/json",
      Authorization: "Token " + TOKEN,
    },
    body: JSON.stringify({
      query: query,
      from_bound: { value: from_bound },
      to_bound: { value: to_bound },
      locations: (() => {
        const res = {};
        console.log(location);
        if (location) {
          if (location.region.isReady) {
            res["region_fias_id"] = location.region.id;
          }
          if (location.area.isReady) {
            res["area_fias_id"] = location.area.id;
          }
          if (location.street.isReady) {
            res["street_fias_id"] = location.street.id;
          }
          if (location.settlement.isReady && location.city.isReady) {
            res["settlement_fias_id"] = location.settlement.id;
            res["city_fias_id"] = location.city.id;
          } else if (!location.city.isReady && location.settlement.isReady) {
            res["settlement_fias_id"] = location.settlement.id;
            res["city_fias_id"] = null;
          } else if (location.city.isReady && !location.settlement.isReady) {
            res["settlement_fias_id"] = null;
            res["city_fias_id"] = location.city.id;
          }
        }
        //console.log(res);
        if (Object.keys(res).length) return [res];
        return null;
      })(),
      restrict_value: true,
    }),
  };

  try {
    const response = await fetch(URL, options);
    const responseJson = await response.json();
    return responseJson;
  } catch (error) {
    console.error(error);
  }
}

export default getData;
