package module6;

import de.fhpotsdam.unfolding.UnfoldingMap;
import de.fhpotsdam.unfolding.data.PointFeature;
import de.fhpotsdam.unfolding.data.ShapeFeature;
import de.fhpotsdam.unfolding.geo.Location;
import de.fhpotsdam.unfolding.marker.Marker;
import de.fhpotsdam.unfolding.marker.SimpleLinesMarker;
import de.fhpotsdam.unfolding.utils.MapUtils;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import parsing.ParseFeed;


/** An applet that shows airports (and routes)
 * on a world map.  
 * @author Adam Setters and the UC San Diego Intermediate Software Development
 * MOOC team
 *
 */
public class AirportMap extends PApplet {
	
	UnfoldingMap map;
	private List<Marker> airportList;
	private List<Marker> routeList;
	private HashMap<String, List<Marker>> routesBySource;
	private CommonMarker lastSelected;
	private CommonMarker lastClicked;
	
	public void setup() {
		// setting up PAppler
		size(800,600, OPENGL);
		
		// setting up map and default events
		map = new UnfoldingMap(this, 50, 50, 750, 550);
		MapUtils.createDefaultEventDispatcher(this, map);
		
		// get features from airport data
		List<PointFeature> features = ParseFeed.parseAirports(this, "airports.dat");
		
		// list for markers, hashmap for quicker access when matching with routes
		airportList = new ArrayList<Marker>();
		HashMap<Integer, Location> airports = new HashMap<Integer, Location>();
		routesBySource = new HashMap<String, List<Marker>>();
		
		// create markers from features
		for(PointFeature feature : features) {
			AirportMarker m = new AirportMarker(feature);
	
			m.setRadius(5);
			airportList.add(m);
			
			// put airport in hashmap with OpenFlights unique id for key
			airports.put(Integer.parseInt(feature.getId()), feature.getLocation());
		
		}
		
		
		// parse route data
		List<ShapeFeature> routes = ParseFeed.parseRoutes(this, "routes.dat");
		routeList = new ArrayList<Marker>();
		for(ShapeFeature route : routes) {
			
			// get source and destination airportIds
			int source = Integer.parseInt((String)route.getProperty("source"));
			int dest = Integer.parseInt((String)route.getProperty("destination"));
			
			// get locations for airports on route
			if(airports.containsKey(source) && airports.containsKey(dest)) {
				route.addLocation(airports.get(source));
				route.addLocation(airports.get(dest));

				SimpleLinesMarker sl = new SimpleLinesMarker(route.getLocations(), route.getProperties());
				sl.setColor(color(220, 220, 220));
				sl.setStrokeWeight(1);
				sl.setHidden(true);
				routeList.add(sl);

				String sourceKey = route.getProperty("source").toString();
				if (!routesBySource.containsKey(sourceKey)) {
					routesBySource.put(sourceKey, new ArrayList<Marker>());
				}
				routesBySource.get(sourceKey).add(sl);
			}
		}
		
		map.addMarkers(routeList);
		map.addMarkers(airportList);
		
	}
	
	public void draw() {
		background(0);
		map.draw();
		
	}

	@Override
	public void mouseMoved()
	{
		if (lastSelected != null) {
			lastSelected.setSelected(false);
			lastSelected = null;
		}

		for (Marker marker : airportList) {
			CommonMarker airport = (CommonMarker) marker;
			if (airport.isInside(map, mouseX, mouseY)) {
				lastSelected = airport;
				airport.setSelected(true);
				return;
			}
		}
	}

	@Override
	public void mouseClicked()
	{
		if (!checkAirportsForClick()) {
			hideAllRoutes();
			lastClicked = null;
		}
	}

	private boolean checkAirportsForClick()
	{
		for (Marker marker : airportList) {
			if (!marker.isHidden() && marker.isInside(map, mouseX, mouseY)) {
				lastClicked = (CommonMarker) marker;
				hideAllRoutes();

				List<Marker> routesFromAirport = routesBySource.get(marker.getId());
				if (routesFromAirport != null) {
					for (Marker route : routesFromAirport) {
						route.setHidden(false);
					}
				}

				return true;
			}
		}

		return false;
	}

	private void hideAllRoutes()
	{
		for (Marker route : routeList) {
			route.setHidden(true);
		}
	}
	

}
