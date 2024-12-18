// let is_login = false; // Отображает вошел ли пользователь в кабинет или нет.
$(function () {
	var $left_menu_popup = $('.left-menu-popup');
	var $right_menu_popup = $('.right-menu-popup');
	let currentLocation = ""; // Для сохранения страницы при переходе на настольную версию.
	
	$(".right-menu-trigger").click(function () {		
		leftMenuClose($left_menu_popup); // Закрыть левое меню.
		rightMenuPopoupClose($right_menu_popup);
		return false;
	});

	$(".left-menu-trigger").click(function () {
		rightMenuClose($right_menu_popup); // Закрыть правое меню.
		// $('body').addClass('body_pointer');
		leftMenuPopoupClose($left_menu_popup);
		return false;
	});
	
	// Открытие закрытие подменю.
	let submenu1State = 0; // Состояние подменю: 0 - закрыто, 1 - открыто.
	$('.submenuTrigger').click(function () {
		if (submenu1State == 0) {
			submenu1State = 1;
			$(this).siblings("ul").css("display", "block");
		} else {
			submenu1State = 0;
			$(this).siblings("ul").css("display", "none");
		}
	});
	// Кэширование. Переход к странице из меню. Анимация меню.
	// Предотвращение ст. поведения пунктов и подпунктов меню.
	$('.left-menu-popup > ul > li > .inner-a').click(function (e) { 
		// Предотвратить стандартное поведение ссылок в меню и подменю.
		e.preventDefault();
		load_page($(this).attr("href"));
		leftMenuClose($left_menu_popup);
	});
	// Кэш. Анимация меню. Предотвращение ст. поведения подпунктов меню и пунктов меню.
	$('.right-menu-popup .submenu > li > a[class="inner-a"], #submenu7Trigger').click(function (e) {
		// Предотвратить стандартное поведение ссылок в меню и подменю.
		// e.stopPropagation();
		e.preventDefault();
		load_page($(this).attr("href"));
		// history.pushState({page: $(e.target).attr("href")}, '', $(e.target).attr("href"));
		history.pushState({page: $(e.target).attr("href")}, '');

		rightMenuPopoupClose($right_menu_popup);
	});
	// Нажатие по логотипу олимпиады - переход на главную страницу.
	$('.logo-a').click(function (e) {
		// Предотвратить стандартное поведение ссылок в меню и подменю.
		e.preventDefault();
		leftMenuClose($left_menu_popup); // Закрыть левое меню.
		rightMenuClose($right_menu_popup); // Закрыть правое меню.
		load_page($(this).attr("href"));
	});

	// $('.submenuTrigger').click(function (e) {
	// 	e.preventDefault();
	// });
	// Левое меню.
	
	$('.loginTrigger').click(function (e) {
		e.preventDefault();
		load_page($(this).attr("href"));
	})
	$('.signupTrigger').click(function (e) {
		e.preventDefault();
		is_login = false;
		load_page($(this).attr("href"));
	})
	window.onpopstate = popState; // Для работы с историей браузера FIXME.
	// console.log("page_id: ", page_id);

	// Сохранение кнопок приглашения к регистрации и входу в кэше.
	console.log("script.js");
	$cache.login_and_signup_btns.allContent = $('#login-and-signup-btns');

	// Для форм login, signup;
	let loginForm = undefined; 
	let signupForm = undefined; 

	// Прикрепление обработчика к кнопке выхода.
	if($('#outbtn').length){
		initLogout('#outbtn');
	}
	
	if(currentPageOnDesktop !== ""){ // Для перехода на мобильную версию с сохранением просматриваемой страницы.
		//alert("0" + currentPageOnDesktop);
		switch(currentPageOnDesktop){		
			case 'index/':
				currentPageOnDesktop = "";
				break;
			case 'cabinet/profile':
				currentPageOnDesktop = "user/login";
				break;
			case 'novice/reg_user':
				currentPageOnDesktop = "user/signup";
				break;
			case 'contact/':
				currentPageOnDesktop = "contact/contact";
				break;  
		}
		//currentPageOnDesktop = "contact/contact";
		//alert("1" + currentPageOnDesktop);
		//load_page(currentPageOnDesktop);
		// setTimeout(loadPage, 500, currentPageOnDesktop);
		
		// load_page(currentPageOnDesktop);
		
		//console.lot("curPageDesk" + currentPageOnDesktop);
		//alert("2" + currentPageOnDesktop);
	}
});
