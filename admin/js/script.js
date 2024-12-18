$(document).ready(function(){
    $('.ob').click(function(){
        var ids = '';
        $('.masscb').each(function(){
            if($(this).attr('checked') == true) {
                if(ids != '') ids = ids + '&';
                ids = ids + 'ids[]=' + $(this).val();
            }
        });
        if(ids != '') location.href = '/olimp_stages_ob.html?' + ids;
    });
    
    $('.massedit').click(function(){
        var ids = '';
        $('.masscb').each(function(){
            if($(this).attr('checked') == true) {
                if(ids != '') ids = ids + '&';
                ids = ids + 'ids[]=' + $(this).val();
            }
        });
        if(ids != '') location.href = '/olimp_stage_mass_edit.html?' + ids;
    });
    
    $('.massdel').click(function(){
        var ids = '';
        $('.masscb').each(function(){
            if($(this).attr('checked') == true) {
                if(ids != '') ids = ids + '&';
                ids = ids + 'ids[]=' + $(this).val();
            }
        });
        if(ids != '') location.href = '/olimp_stage_mass_del.html?' + ids;
    });
    
    $('input[name="exam"], input[name="place"]').click(function(){
        if($(this).attr('checked') == true) $(this).next().show();
        else $(this).next().hide();
    })
    
    $('.step1').click(function(){
        $('[name="place"]').each(function(){
            if($(this).attr('checked') == false){
                $(this).next().find('[type="checkbox"]').each(function(){
                    $(this).attr('checked', false);
                });
            }
        });
    });
    
    $('img.copy').click(function(){
        var name = $(this).prev().attr('data-name');
        var val = $(this).prev().val();
        $('[data-name="'+name+'"]').each(function(){
            $(this).val(val);
        })
    });
});

addEventListener('DOMContentLoaded', function () {
	pickmeup('.calendar', {
		position       : 'bottom',
		hide_on_select : true,
        format	: 'Y-m-d',
        date      : [
			new Date($('.calendar').attr('data-date')),
		],
	});
    
    pickmeup('.calendar2', {
		position       : 'bottom',
		hide_on_select : true,
        format	: 'Y-m-d',
        date      : [
			new Date($('.calendar2').attr('data-date')),
		],
	});
});