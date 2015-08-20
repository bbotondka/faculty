(function ($) {

    // all Javascript code goes here
    jQuery(document).ready(function () {
        jQuery('.nav-trigger').click(function () {
            if (jQuery(this).hasClass('active'))
            {
                jQuery('.nav-container').animate({'left': '-100%'}, 500);
            } else {
                jQuery('.nav-container').animate({'left': '0'}, 500);
            }
            jQuery(this).toggleClass('active');
        })
    })

})(jQuery);
