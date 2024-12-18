/* eslint-disable */

const debounce = (cb, delay = 250) => {
  let timeout;

  return function () {
    const cbCall = () => cb.apply(this, arguments);
    clearTimeout(timeout);
    timeout = setTimeout(cbCall, delay);
  };
};

export default debounce;
