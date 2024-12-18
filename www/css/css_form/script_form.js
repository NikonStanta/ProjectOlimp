    /*---------Add Other Input--------*/
    
    let other_place = document.getElementById('other_place');
    let other_place_input = document.getElementById('other_place_of_residence');
    let other_year = document.getElementById('other_year');
    let other_year_input = document.getElementById('other_year_of_admission_text');
    let other_learn_dod = document.getElementById('other_learn_dod');
    let other_learn_dod_input = document.getElementById('other_learn_dod_text');
    let other_event = document.getElementById('other_learn_mpei');
    let other_event_input = document.getElementById('other_learn_mpei_text');

    other_place.addEventListener('click', () => {
        if (other_place.checked) {
            other_place_input.disabled = false;
            other_place_input.focus();    
                                
            other_place_input.oninput = function() {
                other_place.value = other_place_input.value;
            };
        }
        else {
           // other_place_input.disabled = true; //по хорошему должно обратно становится недоступным
            console.log('Checkbox is not checked');
        }
    });


    other_year.addEventListener('click', () => {
        if (other_year.checked) {
            other_year_input.disabled = false;
            other_year_input.focus();
            
            other_year_input.oninput = function() {
                other_yaer.value = other_year_input.value;
            };
        }
    });

    other_learn_dod.addEventListener('click', () => {
        if (other_learn_dod.checked) {
            other_learn_dod_input.disabled = false;
            other_learn_dod_input.focus();
          
             other_learn_dod_input.oninput = function() {
                 other_learn_dod.value = other_learn_dod_input.value;
             };
        }
    });

    other_event.addEventListener('click', () => {
        if (other_event.checked) {
            other_event_input.disabled = false;
            other_event_input.focus();
          
            other_event_input.oninput = function() {
                other_event.value = other_event_input.value;
            };
        }
    });


// /*--- Get input value ---*/
// let surname = document.getElementById('surname').value;
// let name = document.getElementById('name').value;
// /*----Block the button Submit ----*/
// let sub_button = document.getElementById('btnCheck');
// let cansubmit = true;
// sub_button.disabled = cansubmit;
// sub_button.addEventListener('submit', () => {
//     if (( surname != "" && surname != null) && ( name != "" && name != null)) {
//         cansubmit = false;    
//     }
// })