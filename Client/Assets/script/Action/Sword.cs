using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sword : MonoBehaviour {
    public GameObject swordPrefab;
    public float swingSpeed;
    public float lifeTime;

    bool running = false;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void Run()
    {
        if (!running)
        {
            running = true;
            StartCoroutine(SwordCoroutine());
        }
    }

    IEnumerator SwordCoroutine()
    {
        var sword = GameObject.Instantiate(swordPrefab);

        sword.transform.position = transform.position;
        sword.transform.rotation = transform.rotation;
        sword.transform.SetParent(transform);
        
        float accumTime = 0;
        for (;accumTime < lifeTime;)
        {
            accumTime += Time.deltaTime;
            sword.transform.Rotate(Vector3.back * Time.deltaTime * swingSpeed);
            yield return null;
        }

        Destroy(sword);
        running = false;
    }
}
