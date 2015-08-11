Install as usual, should be under Other package right below Browscap.
 
This module provides compatibilty with the Mobile Switch modules developer tools:
http://drupal.org/project/mobile_switch

Since the core Drupal cache provider uses the URL as the cache id
it's possible to get incorrect pages served from the cache when
using the Browscap Block module if your mobile and non-mobile pages
share the same URL. Browscap Block comes with a replacement for the
core Drupal database caching provider that appends the theme and an
indicator specifying whether the request is from a mobile device or
not to the cache id. To use this new cache provider for the page
cache simply add the following to the end of your settings.php file.

$conf['cache_backends'][] = 'sites/all/modules/contrib/browscap_block/browscap_block_cache.inc';
$conf['cache_class_cache_page'] = 'BrowscapBlockDatabaseCache';

Just make sure the path to the Browscap Block module directory
matches your installation.
