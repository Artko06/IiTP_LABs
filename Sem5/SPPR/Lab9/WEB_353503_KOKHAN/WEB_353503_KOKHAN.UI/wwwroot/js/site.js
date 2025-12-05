$(document).ready(function () {
    $('#ajax-content').on('click', '.page-link, .dropdown-item', function (e) {
        e.preventDefault();

        var url = $(this).attr('href');
        if (!url) return;

        $('#ajax-content').html('<div class="text-center"><div class="spinner-border" role="status"></div></div>');

        $.ajax({
            url: url,
            type: 'GET',
            headers: { 'X-Requested-With': 'XMLHttpRequest' },
            success: function (data) {
                $('#ajax-content').html(data);
                history.pushState(null, '', url);
            },
            error: function (xhr, status, error) {
                console.error('AJAX Error:', error);
                window.location.href = url;
            }
        });
    });
});